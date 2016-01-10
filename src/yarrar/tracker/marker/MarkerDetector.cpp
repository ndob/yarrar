#include "MarkerDetector.h"
#include "Util.h"

#include <opencv2/opencv.hpp>
#include <array>
#include <iostream>
#include <cmath>
#include <vector>

namespace
{

enum Hierarchy
{
    HIERARCHY_NO_VALUE = -1,
    HIERARCHY_NEXT = 0,
    HIERARCHY_PREVIOUS = 1,
    HIERARCHY_FIRST_CHILD = 2,
    HIERARCHY_PARENT = 3
};

const cv::Scalar RED = cv::Scalar(0, 0, 255);
const cv::Scalar GREEN = cv::Scalar(0, 255, 0);
const cv::Scalar BLUE = cv::Scalar(255, 0, 0);

// FIXME: To use extrinsic guess with multiple poses,
// a history buffer needs to be implemented.
const bool PNP_USE_EXTRINSIC_GUESS = false;
const uint64 RANDOM_SEED = 12345;
}

namespace yarrar
{

using namespace cv;

MarkerDetector::MarkerDetector(const Config& config)
    : m_config(config)
    , m_rng(RANDOM_SEED)
    , m_poseRotation(3, 1, cv::DataType<double>::type)
    , m_poseTranslation(3, 1, cv::DataType<double>::type)
{
    assert(m_config.trackingResolution.width > 0);
    assert(m_config.trackingResolution.height > 0);
}

std::vector<Marker> MarkerDetector::findMarkers(const Mat& binaryImage)
{
    std::vector<std::vector<Point>> foundContours;
    std::vector<std::vector<Point2f>> validRectangles;
    std::vector<int> validHierarchyIndices;

    // Hierarchy is stored as an array with values:
    // [Next, Previous, First_Child, Parent]
    // -1 corresponds to no value.
    std::vector<cv::Vec4i> hierarchy;

    // Find contours of possible markers.
    findContours(binaryImage, foundContours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

    int hierarchyIdx = -1;
    for(const auto& contour : foundContours)
    {
        hierarchyIdx++;
        // Ignore too small contours.
        if(contourArea(contour, false) < m_config.contourAreaMinSize) continue;

        // Calculate outer perimeter for contour.
        bool isClosed = true;
        double perimeterLen = arcLength(contour, isClosed);

        // Approximate contour as polygon.
        std::vector<Point2f> approx;
        approxPolyDP(contour, approx, 0.05 * perimeterLen, isClosed);

        // Rectangles have exactly 4 corner points.
        if(approx.size() != 4) continue;

        // Rectangles can't be convex.
        if(!isContourConvex(approx)) continue;

        validRectangles.push_back(approx);
        validHierarchyIndices.push_back(hierarchyIdx);
    }

    // Search for valid markers.
    std::vector<Marker> ret;
    std::vector<int> usedIndices;
    for(size_t i = 0; i < validRectangles.size(); ++i)
    {
        // Do not try to find markers inside of already used inner/outer rectangles.
        if(util::contains(usedIndices, validHierarchyIndices[i])) continue;

        // Prevent full marker nesting.
        // ie. The whole marker inside some previous inner rectangle.
        if(util::contains(usedIndices,
               hierarchy[validHierarchyIndices[i]][HIERARCHY_PARENT]))
        {
            continue;
        }

        bool foundInnerRectangle = false;
        double outerPerimeterLength = arcLength(validRectangles[i], true);
        double greatestInnerPerimeterLength = 0.0;
        size_t greatestIndex = 0;

        // Get OpenCV hierarchy-value for this contour.
        auto outerHier = hierarchy[validHierarchyIndices[i]];
        int childIdx = outerHier[HIERARCHY_FIRST_CHILD];

        // Find the largest rectangle contour inside current outer contour.
        while(childIdx != HIERARCHY_NO_VALUE)
        {
            // Check if this child is a valid rectangle contour.
            for(size_t j = 0; j < validHierarchyIndices.size(); ++j)
            {
                if(validHierarchyIndices[j] == childIdx)
                {
                    double innerPerimeterLength = arcLength(validRectangles[j], true);

                    if(innerPerimeterLength < outerPerimeterLength &&
                        innerPerimeterLength > (m_config.innerRectangleMinimumSize * outerPerimeterLength) &&
                        greatestInnerPerimeterLength < innerPerimeterLength)
                    {
                        greatestInnerPerimeterLength = innerPerimeterLength;
                        greatestIndex = j;
                        foundInnerRectangle = true;
                    }
                }
            }
            // Check next child, if available.
            childIdx = hierarchy[childIdx][HIERARCHY_NEXT];
        }

        // If child rectangle was found, add a marker.
        if(foundInnerRectangle)
        {
            Marker m = {
                validRectangles[greatestIndex],
                validRectangles[i]
            };
            ret.push_back(m);
            usedIndices.push_back(validHierarchyIndices[greatestIndex]);
            usedIndices.push_back(validHierarchyIndices[i]);
        }
    }

    return ret;
}

cv::Mat MarkerDetector::getRectifiedInnerImage(const std::vector<cv::Point2f>& imagePoints,
    const cv::Mat& image, const int imageSize)
{
    std::vector<Point2f> corners;

    corners.push_back(Point2f(imageSize, 0));
    corners.push_back(Point2f(imageSize, imageSize));
    corners.push_back(Point2f(0, imageSize));
    corners.push_back(Point2f(0, 0));

    // Find perspective transform for image points.
    Mat transform = findHomography(imagePoints, corners, CV_RANSAC);
    cv::Mat rectified(image.size(), image.type());
    cv::warpPerspective(image, rectified, transform, image.size());

    // Select only the part of the image, which contains marker id.
    Mat subImage(rectified, cv::Rect(0, 0, imageSize, imageSize));

    return subImage;
}

Pose MarkerDetector::getPose(const std::vector<cv::Point2f>& contour)
{
    estimatePnP(contour);

    return {
        m_poseRotation,
        m_poseTranslation,
        getCameraMatrix(),
        -1
    };
}

Mat MarkerDetector::getCameraMatrix()
{
    Mat cameraMatrix(3, 3, DataType<float>::type);

    const float horizFOVDegrees = 50.0f;
    const float horizFOVRadians = horizFOVDegrees / 2.0f * (static_cast<float>(M_PI) / 180.0f);

    const float cx = static_cast<float>(m_config.trackingResolution.width) / 2.0f;
    const float cy = static_cast<float>(m_config.trackingResolution.height) / 2.0f;
    const float fx = cx / std::tan(horizFOVRadians);
    const float fy = fx;

    cameraMatrix.at<float>(0, 0) = fx;
    cameraMatrix.at<float>(0, 1) = 0;
    cameraMatrix.at<float>(0, 2) = cx;

    cameraMatrix.at<float>(1, 0) = 0;
    cameraMatrix.at<float>(1, 1) = fy;
    cameraMatrix.at<float>(1, 2) = cy;

    cameraMatrix.at<float>(2, 0) = 0;
    cameraMatrix.at<float>(2, 1) = 0;
    cameraMatrix.at<float>(2, 2) = 1;

    return cameraMatrix;
}

Mat MarkerDetector::getDistCoeffs()
{
    Mat distCoeffs(4, 1, cv::DataType<float>::type);
    distCoeffs.at<float>(0) = 0;
    distCoeffs.at<float>(1) = 0;
    distCoeffs.at<float>(2) = 0;
    distCoeffs.at<float>(3) = 0;

    return distCoeffs;
}

void MarkerDetector::estimatePnP(const std::vector<Point2f>& corners)
{
    std::vector<Point3f> objectPoints;

    /*
     * p1 --- p0
     * |       |
     * |       |
     * p2 --- p3
     */
    objectPoints.push_back(Point3f(1, 1, 0));
    objectPoints.push_back(Point3f(-1, 1, 0));
    objectPoints.push_back(Point3f(-1, -1, 0));
    objectPoints.push_back(Point3f(1, -1, 0));

    cv::solvePnPRansac(objectPoints, corners, getCameraMatrix(), getDistCoeffs(),
        m_poseRotation, m_poseTranslation, PNP_USE_EXTRINSIC_GUESS);
}

void MarkerDetector::drawAxes(const Mat& image, const Mat& rvec, const Mat& tvec)
{
    std::vector<Point3f> linepoints;
    linepoints.push_back(Point3f(0, 0, 0));
    linepoints.push_back(Point3f(1, 0, 0));
    linepoints.push_back(Point3f(0, 1, 0));
    linepoints.push_back(Point3f(0, 0, 1));

    std::vector<Point2f> projectedPoints;
    cv::projectPoints(linepoints, rvec, tvec, getCameraMatrix(), getDistCoeffs(), projectedPoints);

    // X-axis
    line(image, projectedPoints[0], projectedPoints[1], RED, 2, 8);
    // Y-axis
    line(image, projectedPoints[0], projectedPoints[2], GREEN, 2, 8);
    // Z-axis
    line(image, projectedPoints[0], projectedPoints[3], BLUE, 2, 8);
}

void MarkerDetector::drawPolygon(const std::vector<cv::Point2f>& vertices, const Mat& image)
{
    Scalar color = Scalar(m_rng.uniform(0, 255), m_rng.uniform(0, 255), m_rng.uniform(0, 255));

    std::vector<Point> corners;
    for(const auto& vertex : vertices)
    {
        corners.push_back(Point(static_cast<int>(vertex.x), static_cast<int>(vertex.y)));
    }

    std::vector<std::vector<Point>> polygons;
    polygons.push_back(corners);
    drawContours(image, polygons, -1, color, 2);
}
}
