#pragma once

#include "HistoryBuffer.h"
#include "MarkerParser.h"
#include "Pipeline.h"
#include "Types.h"

#include <map>

namespace yarrar
{

struct Marker
{
    std::vector<cv::Point2f> innerContour;
    std::vector<cv::Point2f> outerContour;
};


class MarkerDetector
{
public:
    struct Config
    {
        cv::Size trackingResolution;
        double contourAreaMinSize;
        float innerRectangleMinimumSize;
    };

    MarkerDetector(const Config& config);

    std::vector<Marker> findMarkers(const cv::Mat& image);
    cv::Mat getRectifiedInnerImage(const std::vector<cv::Point2f>& imagePoints,
        const cv::Mat& image,
        const int imageSize);
    Pose getPose(const int coordinateSystemId, const std::vector<cv::Point2f>& contour);
    void pruneHistory(const std::vector<int>& usedCoordinateSystems);

    void drawAxes(const cv::Mat& image, const cv::Mat& rvec, const cv::Mat& tvec);
    void drawPolygon(const std::vector<cv::Point2f>& vertices, const cv::Mat& image);

private:
    Pose estimatePnP(const int coordinateSystemId, const std::vector<cv::Point2f>& corners);
    cv::Mat getCameraMatrix();
    cv::Mat getDistCoeffs();

    Config m_config;
    cv::RNG m_rng;
    std::map<int, HistoryBuffer<Pose>> m_poseHistories;
};
}
