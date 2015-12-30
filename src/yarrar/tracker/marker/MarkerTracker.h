#pragma once

#include "Pipeline.h"
#include "MarkerDetector.h"

#define DEBUG_DRAWING 1

namespace yarrar {

template <typename ParserType>
class MarkerTracker: public Tracker
{
public:
    MarkerTracker(int width, int height):
        m_detector(width, height)
    {
        static_assert(std::is_base_of<MarkerParser, ParserType>::value, "ParserType must derive from MarkerParser.");
    }

    Pose getPose(const cv::Mat& image) override
    {
        using namespace cv;

        Mat binary;
        {
            Mat resizedColored, gray;
            // Downscale and convert to gray scale.
            resize(image, resizedColored, m_detector.getTrackingResolution());
            cvtColor(resizedColored, gray, CV_BGR2GRAY);

            // Mark areas that are between totally black (0) and gray
            // threshold (100) with black, others with white. Tracking
            // and id-detection is done with this Mat.
            // TODO: Threshold should be configurable.
            inRange(gray, 0, 100, binary);
        }

        Pose ret;
        ret.valid = false;
        auto markers = m_detector.findMarkers(binary);

        if(markers.size() > 0)
        {
            for(const auto& marker : markers)
            {
                ret = m_detector.getPose(marker.outer);

                Mat rectified = m_detector.getRectifiedInnerImage(marker.inner, binary);
                MarkerValue value = m_parser.getData(rectified);
                ret.coordinateSystemId = value.id;

                // Check if marker data indicates that the marker is rotated around z-axis.
                if(value.zRotation != MarkerRotation::DEG_0)
                {
                    double alpha;
                    switch(value.zRotation)
                    {
                        case MarkerRotation::DEG_0:
                            alpha = 0;
                            break;
                        case MarkerRotation::DEG_90:
                            alpha = M_PI_2;
                            break;
                        case MarkerRotation::DEG_180:
                            alpha = M_PI;
                            break;
                        case MarkerRotation::DEG_270:
                            alpha = M_PI + M_PI_2;
                            break;
                    }

                    // Construct rotation matrix (z-axis).
                    Mat Rz = (Mat_<double>(3, 3) <<
                        std::cos(alpha), -std::sin(alpha), 0.0,
                        std::sin(alpha),  std::cos(alpha), 0.0,
                        0.0, 0.0, 1.0);

                    cv::Mat rotationMatrix;
                    // Convert pose rotation to matrix.
                    cv::Rodrigues(ret.rotation, rotationMatrix);
                    // Convert rotation matrix back to Rodrigues' short form.
                    cv::Rodrigues(rotationMatrix * Rz, ret.rotation);
                }

#if defined(DEBUG_DRAWING)
                m_detector.drawPolygon(marker.inner, image);
                m_detector.drawPolygon(marker.outer, image);
                m_detector.drawAxes(image, ret.rotation, ret.translation);
#endif
            }
        }

        return ret;
    }

private:

    MarkerDetector m_detector;
    ParserType m_parser;
};

}
