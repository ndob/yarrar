#include "MarkerDetector.h"

#include <opencv2/opencv.hpp>
#include <array>
#include <iostream>
#include <math.h>
#include <vector>

namespace {

const cv::Scalar RED = cv::Scalar(0, 0, 255);
const cv::Scalar GREEN = cv::Scalar(0, 255, 0);
const cv::Scalar BLUE = cv::Scalar(255, 0, 0);
const bool PNP_USE_EXTRINSIC_GUESS = true;
const uint64 RANDOM_SEED = 12345;

}

namespace yarrar {

using namespace cv;

MarkerDetector::MarkerDetector(int width, int height):
    m_width(width),
    m_height(height),
    m_rng(RANDOM_SEED),
    m_poseRotation(3,1,cv::DataType<double>::type),
    m_poseTranslation(3,1,cv::DataType<double>::type)
{
}

Pose MarkerDetector::getPose(const cv::Mat& image)
{
    Pose ret;
    ret.valid = false;
    auto markers = findMarkers(image);

    if(markers.size() > 0)
    {
        for(const auto& marker : markers)
        {
            int id = parseId(marker.inner, image);
            estimatePnP(marker.outer);
            ret = {
                m_poseRotation,
                m_poseTranslation,
                getCameraMatrix(),
                id,
                true
            };

            // Debugging
            drawPolygon(marker.inner, image);
            drawPolygon(marker.outer, image);
            drawAxes(image, m_poseRotation, m_poseTranslation);
        }
    }

    return ret;
}

std::vector<Marker> MarkerDetector::findMarkers(const Mat& image)
{
    Mat detected;
    Mat gray;
    std::vector<std::vector<Point>> foundContours;
    std::vector<std::vector<Point2f>> validRectangles;
    std::vector<int> validHierarchyIndices;

    // Hierarchy is stored as an array with values:
    // [Next, Previous, First_Child, Parent]
    // -1 corresponds to no value.
    std::vector<cv::Vec4i> hierarchy;

    // Convert original to gray scale.
    cvtColor(image, gray, CV_BGR2GRAY);

    // Mark areas that are between totally black (0) and gray(255/2)
    // with black, others with white.
    inRange(gray, 0, 255/2, detected);

    // Find contours for the areas detected in previous step.
    findContours(detected, foundContours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

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
    for(int i = 0; i < validRectangles.size(); ++i)
    {
        double outerPerimeterLength = arcLength(validRectangles[i], true);
        double greatestInnerPerimeterLength = 0.0;
        int greatestIndex = -1;

        // Get OpenCV hierarchy-value for this contour.
        auto outerHier = hierarchy[validHierarchyIndices[i]];
        int childIdx = outerHier[2];

        // Find the largest rectangle contour inside current outer contour.
        while(childIdx != -1)
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
            childIdx = hierarchy[childIdx][0];
        }

        // If child rectangle was found, add a marker.
        if(greatestIndex != -1)
        {
            Marker m = {
                validRectangles[greatestIndex],
                validRectangles[i]
            };
            ret.push_back(m);
        }
    }

    return ret;
}

int MarkerDetector::parseId(const std::vector<Point2f>& imagePoints, const Mat& image)
{
    // Size for image that will be parsed.
    const int imageSize = 100;

    std::vector<Point2f> corners;
    corners.push_back(Point2f(imageSize,imageSize));
    corners.push_back(Point2f(0,imageSize));
    corners.push_back(Point2f(0,0));
    corners.push_back(Point2f(imageSize,0));

    // Find perspective transform for image points.
    Mat transform = findHomography(imagePoints, corners, CV_RANSAC );

    cv::Mat rectified(image.size(), image.type());
    cv::warpPerspective(image, rectified, transform, image.size());

    // Select only the part of the image, which contains marker id.
    Mat subImage(rectified, cv::Rect(0, 0, imageSize, imageSize));
    cvtColor(subImage, subImage, CV_BGR2GRAY);
    Mat binary;
    inRange(subImage, 0, 255/2, binary);

    // Works by dividing the image to 8x8 matrix (byte per line).
    // Then check if a cell is either black (1) or white (0).
    const int dimension = 8;
    const int stepSize = imageSize / dimension;
    const int start = stepSize / 2;
    std::array<std::array<bool, 8>, 8> field;

    for(int i = 0; i < dimension; ++i)
    {
        for(int j = 0; j < dimension; ++j)
        {
            const int col = start + i * stepSize;
            const int row = start + j * stepSize;
            const bool isBlack = binary.at<uchar>(col, row) > 0;
            field[i][j] = isBlack;
        }
    }

    // TODO: Determine orientation from field.

    // TODO: A real parser. This is only for testing.
    // Final id is fourth line as binary converted to integer.
    unsigned char id = 0;
    for(int i = 0; i < field[3].size(); ++i)
    {
        if(field[3][i])
        {
            id = id | (unsigned char) (1 << (7 - i));
        }
    }

    return id;
}

Mat MarkerDetector::getCameraMatrix()
{
    Mat cameraMatrix(3,3,DataType<float>::type);

    const float horizFOVDegrees = 50.0f;
    const float horizFOVRadians = horizFOVDegrees / 2.0f * (static_cast<float>(M_PI) / 180.0f);

    const float cx = static_cast<float> (m_width) / 2.0f;
    const float cy = static_cast<float> (m_height) / 2.0f;
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
