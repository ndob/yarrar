#pragma once

#include "Pipeline.h"
#include "Pose.h"

namespace yarrar
{

struct Marker
{
    std::vector<cv::Point2f> inner;
    std::vector<cv::Point2f> outer;
};

class MarkerDetector: public Detector
{
public:
    MarkerDetector(int width, int height);

    Pose getPose(const cv::Mat& rawData) override;
private:
    std::vector<Marker> findMarkers(const cv::Mat& image);
    int parseId(const std::vector<cv::Point2f>& imagePoints, const cv::Mat& image);
    void estimatePnP(const std::vector<cv::Point2f>& corners);

    cv::Mat getCameraMatrix();
    cv::Mat getDistCoeffs();

    void drawAxes(const cv::Mat& image, const cv::Mat& rvec, const cv::Mat& tvec);
    void drawPolygon(const std::vector<cv::Point2f>& vertices, const cv::Mat& image);

    cv::Size m_trackingResolution;
    cv::RNG m_rng;
    cv::Mat m_poseRotation;
    cv::Mat m_poseTranslation;
};

}

