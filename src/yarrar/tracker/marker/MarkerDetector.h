#pragma once

#include "Pipeline.h"
#include "Pose.h"
#include "MarkerParser.h"

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
    MarkerDetector(const cv::Size& trackingResolution);

    std::vector<Marker> findMarkers(const cv::Mat& image);
    cv::Mat getRectifiedInnerImage(const std::vector<cv::Point2f>& imagePoints, const cv::Mat& image);
    Pose getPose(const std::vector<cv::Point2f>& contour);

    void drawAxes(const cv::Mat& image, const cv::Mat& rvec, const cv::Mat& tvec);
    void drawPolygon(const std::vector<cv::Point2f>& vertices, const cv::Mat& image);

private:
    void estimatePnP(const std::vector<cv::Point2f>& corners);
    cv::Mat getCameraMatrix();
    cv::Mat getDistCoeffs();

    cv::Size m_trackingResolution;
    cv::RNG m_rng;
    cv::Mat m_poseRotation;
    cv::Mat m_poseTranslation;
};

}
