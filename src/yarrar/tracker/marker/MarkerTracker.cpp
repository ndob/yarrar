#include "MarkerTracker.h"

namespace
{

const json11::Json::shape CONFIG_SHAPE{
    { "parser", json11::Json::STRING },
    { "tracking_resolution_width", json11::Json::NUMBER }
};
}

namespace yarrar
{

MarkerTracker::MarkerTracker(int width, int height, const json11::Json& config)
    : Tracker(config)
{
    std::string err;
    if(!config.has_shape(CONFIG_SHAPE, err))
    {
        throw std::runtime_error(std::string("MarkerTracker: ") + err);
    }

    const int preferredTrackingResolutionWidth = config["tracking_resolution_width"].int_value();
    m_trackingResolution = util::getScaledDownResolution(width,
        height,
        preferredTrackingResolutionWidth);

    m_detector.reset(new MarkerDetector(m_trackingResolution));

    if(config["parser"].string_value() == "yarrar_parser")
    {
        m_parser.reset(new YarrarMarkerParser);
    }

    assert(m_parser);
}

DatatypeFlags MarkerTracker::depends()
{
    return RGB_CAMERA_FLAG;
}

void MarkerTracker::getPoses(const cv::Mat& image, std::vector<Pose>& output)
{
    using namespace cv;

    Mat binary, gray;
    {
        Mat resizedColored;
        // Downscale and convert to gray scale.
        resize(image, resizedColored, m_trackingResolution);
        cvtColor(resizedColored, gray, CV_BGR2GRAY);

        // Mark areas that are between totally black (0) and gray
        // threshold (100) with black, others with white. Tracking
        // and id-detection is done with this Mat.
        // TODO: Threshold should be configurable.
        inRange(gray, 0, 100, binary);
    }

    // TODO: Find markers modifies the binary image,
    // so it should probably be copied.
    auto markers = m_detector->findMarkers(binary);

    for(const auto& marker : markers)
    {
        Pose cameraPose = m_detector->getPose(marker.outerContour);
        Mat rectified = m_detector->getRectifiedInnerImage(marker.innerContour, gray);
        MarkerValue value = m_parser->getData(rectified);

        if(!value.valid) continue;

        cameraPose.coordinateSystemId = value.id;

        // Check if marker data indicates that the marker is rotated around z-axis.
        if(value.zRotation != Rotation90::DEG_0)
        {
            double alpha = 0.0;
            switch(value.zRotation)
            {
                case Rotation90::DEG_90:
                    alpha = -M_PI_2;
                    break;
                case Rotation90::DEG_180:
                    alpha = -M_PI;
                    break;
                case Rotation90::DEG_270:
                    alpha = -M_PI - M_PI_2;
                    break;
                default:
                    break;
            }

            // Construct rotation matrix (z-axis).
            const Mat Rz = (Mat_<double>(3, 3) << std::cos(alpha), -std::sin(alpha), 0.0,
                std::sin(alpha), std::cos(alpha), 0.0,
                0.0, 0.0, 1.0);

            cv::Mat rotationMatrix;
            // Convert pose rotation to matrix.
            cv::Rodrigues(cameraPose.rotation, rotationMatrix);
            // Convert rotation matrix back to Rodrigues' short form.
            cv::Rodrigues(rotationMatrix * Rz, cameraPose.rotation);

#if defined(DEBUG_DRAWING)
            m_detector->drawPolygon(marker.innerContour, image);
            m_detector->drawPolygon(marker.outerContour, image);
            m_detector->drawAxes(image, cameraPose.rotation, cameraPose.translation);
#endif
        }

        output.push_back(cameraPose);
    }
}
}
