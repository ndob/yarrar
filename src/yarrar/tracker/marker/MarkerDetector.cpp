#include "MarkerDetector.h"

#include <opencv2/opencv.hpp>
#include <array>
#include <iostream>
#include <cmath>
#include <vector>

namespace {

const cv::Scalar RED = cv::Scalar(0, 0, 255);
const cv::Scalar GREEN = cv::Scalar(0, 255, 0);
const cv::Scalar BLUE = cv::Scalar(255, 0, 0);
const bool PNP_USE_EXTRINSIC_GUESS = true;
const int PREFERRED_TRACKING_RESOLUTION_WIDTH = 320;
const uint64 RANDOM_SEED = 12345;

std::pair<int,int> getScaledDown(int width, int height)
{
    if(width <= PREFERRED_TRACKING_RESOLUTION_WIDTH)
    {
        std::pair<int, int> (width, height);
    }
    float aspect = static_cast<float> (width) / static_cast<float> (height);

    return std::pair<int, int> (
        PREFERRED_TRACKING_RESOLUTION_WIDTH, 
        std::floor(PREFERRED_TRACKING_RESOLUTION_WIDTH / aspect)
    );
};

}

namespace yarrar {

using namespace cv;

MarkerDetector::MarkerDetector(int width, int height):
    m_rng(RANDOM_SEED),
    m_poseRotation(3,1,cv::DataType<double>::type),
    m_poseTranslation(3,1,cv::DataType<double>::type)
{
    assert(width > 0);
    assert(height > 0);
    auto scaled = getScaledDown(width, height);
    m_trackingResolution.width = scaled.first;
    m_trackingResolution.height = scaled.second;
}

cv::Size MarkerDetector::getTrackingResolution()
{
    return m_trackingResolution;
}

std::vector<Marker> MarkerDetector::findMarkers(const Mat& binaryImage)
{
    std::vector<std::vector<Point>> foundContours;
    std::vector<std::vector<Point2f>> validRectangles;
    std::vector<int> validHierarchyIndices;

    // Hierarchy is stored as an array with values:
    // [Next, Previous, First_Child, Parent]
    // -1 corresponds to no value.
    static const int NO_VALUE = -1;
    static const size_t HIERARCHY_NEXT = 0;
    static const size_t HIERARCHY_PREVIOUS = 1;
    static const size_t HIERARCHY_FIRST_CHILD = 2;
    static const size_t HIERARCHY_PARENT = 3;
    std::vector<cv::Vec4i> hierarchy;

    // Find contours for the areas detected in previous step.
    findContours(binaryImage, foundContours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

    int hierarchyIdx = -1;
    for(const auto& contour : foundContours)
    {
        hierarchyIdx++;
        // Ignore too small contours.
        if(contourArea(contour, false) < 100) continue;

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
    for(int i = 0; i < validRectangles.size(); ++i)
    {
        // Do not try to find markers inside of already used inner/outer rectangles.
        if(contains(usedIndices, validHierarchyIndices[i])) continue;

        // Prevent full marker nesting. 
        // ie. The whole marker inside some previous inner rectangle.
        if(contains(usedIndices, hierarchy[validHierarchyIndices[i]][HIERARCHY_PARENT])) continue;

        double outerPerimeterLength = arcLength(validRectangles[i], true);
        double greatestInnerPerimeterLength = 0.0;
        int greatestIndex = NO_VALUE;

        // Get OpenCV hierarchy-value for this contour.
        auto outerHier = hierarchy[validHierarchyIndices[i]];
        int childIdx = outerHier[HIERARCHY_FIRST_CHILD];

        // Find the largest rectangle contour inside current outer contour.
        while(childIdx != NO_VALUE)
        {
            // Check if this child is a valid rectangle contour.
            for(int j = 0; j < validHierarchyIndices.size(); ++j)
            {
                if(validHierarchyIndices[j] == childIdx)
                {
                    double innerPerimeterLength = arcLength(validRectangles[j], true);

                    if(innerPerimeterLength < outerPerimeterLength &&
                       innerPerimeterLength > (0.6f * outerPerimeterLength) &&
                       greatestInnerPerimeterLength < innerPerimeterLength)
                    {
                        greatestInnerPerimeterLength = innerPerimeterLength;
                        greatestIndex = j;
                    }
                }
            }
            // Check next child, if available.
            childIdx = hierarchy[childIdx][HIERARCHY_NEXT];
        }

        // If child rectangle was found, add a marker.
        if(greatestIndex != NO_VALUE)
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

cv::Mat MarkerDetector::getRectifiedInnerImage(const std::vector<cv::Point2f>& imagePoints, const cv::Mat& image)
{
    // Size for image that will be parsed.
    const int imageSize = 100;

    std::vector<Point2f> corners;
    corners.push_back(Point2f(imageSize,imageSize));
    corners.push_back(Point2f(0,imageSize));
    corners.push_back(Point2f(0,0));
    corners.push_back(Point2f(imageSize,0));

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
        -1,
        true
    };
}

Mat MarkerDetector::getCameraMatrix()
{
    Mat cameraMatrix(3,3,DataType<float>::type);

    const float horizFOVDegrees = 50.0f;
    const float horizFOVRadians = horizFOVDegrees / 2.0f * (static_cast<float>(M_PI) / 180.0f);

    const float cx = static_cast<float> (m_trackingResolution.width) / 2.0f;
    const float cy = static_cast<float> (m_trackingResolution.height) / 2.0f;
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
    Mat distCoeffs(4,1,cv::DataType<float>::type);
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
    objectPoints.push_back(Point3f(1,1,0));
    objectPoints.push_back(Point3f(-1,1,0));
    objectPoints.push_back(Point3f(-1,-1,0));
    objectPoints.push_back(Point3f(1,-1,0));

    cv::solvePnPRansac(objectPoints, corners, getCameraMatrix(), getDistCoeffs(),
                       m_poseRotation, m_poseTranslation, PNP_USE_EXTRINSIC_GUESS);
};

void MarkerDetector::drawAxes(const Mat& image, const Mat& rvec, const Mat& tvec)
{
    std::vector<Point3f> linepoints;
    linepoints.push_back(Point3f(0,0,0));
    linepoints.push_back(Point3f(1,0,0));
    linepoints.push_back(Point3f(0,1,0));
    linepoints.push_back(Point3f(0,0,1));

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
    Scalar color = Scalar(m_rng.uniform(0,255), m_rng.uniform(0,255), m_rng.uniform(0,255));

    std::vector<Point> corners;
    for(const auto& vertex : vertices)
    {
        corners.push_back(Point(static_cast<int> (vertex.x), static_cast<int> (vertex.y)));
    }

    std::vector<std::vector<Point>> polygons;
    polygons.push_back(corners);
    drawContours(image, polygons, -1, color, 2);
}

}
