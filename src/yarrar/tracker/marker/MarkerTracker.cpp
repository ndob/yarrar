#include "MarkerTracker.h"

namespace
{

const std::string PARSER = "parser";
const std::string YARRAR_PARSER = "yarrar_parser";
const std::string TRACKING_RESOLUTION_WIDTH = "tracking_resolution_width";
const std::string CONTOUR_MIN_AREA_SIZE = "contour_min_area_size";
const std::string INNER_RECTANGLE_MIN_SIZE = "inner_rectangle_min_size";
const std::string MARKER_PARSER_IMAGE_SIZE = "marker_parser_image_size";
const std::string BINARY_IMAGE_THRESHOLD = "binary_image_threshold";

const json11::Json::shape CONFIG_SHAPE{
    { PARSER, json11::Json::STRING },
    { TRACKING_RESOLUTION_WIDTH, json11::Json::NUMBER }
};
}

namespace yarrar
{

MarkerTracker::MarkerTracker(int width, int height, const json11::Json& config)
    : Tracker(config)
{
    applyJsonConfig(config, width, height);

    m_detector.reset(new MarkerDetector(m_config.detectorConfig));

    if(config[PARSER].string_value() == YARRAR_PARSER)
    {
        m_parser.reset(new YarrarMarkerParser);
    }

    assert(m_parser);
}

void MarkerTracker::applyJsonConfig(const json11::Json& config, int width, int height)
{
    std::string err;
    if(!config.has_shape(CONFIG_SHAPE, err))
    {
        throw std::runtime_error(std::string("MarkerTracker: ") + err);
    }

    const int preferredTrackingResolutionWidth = config[TRACKING_RESOLUTION_WIDTH].int_value();
    cv::Size trackingResolution = util::getScaledDownResolution(width,
        height,
        preferredTrackingResolutionWidth);

    // Default configuration values.
    m_config = {
        { trackingResolution, 100, 0.6f },
        100,
        100
    };

    if(config[CONTOUR_MIN_AREA_SIZE].is_number())
    {
        m_config.detectorConfig.contourAreaMinSize = config[CONTOUR_MIN_AREA_SIZE].number_value();
    }

    if(config[INNER_RECTANGLE_MIN_SIZE].is_number())
    {
        m_config.detectorConfig.innerRectangleMinimumSize = static_cast<float>(
            config[INNER_RECTANGLE_MIN_SIZE].number_value());
    }

    if(config[MARKER_PARSER_IMAGE_SIZE].is_number())
    {
        m_config.markerParserImageSize = static_cast<int>(
            config[MARKER_PARSER_IMAGE_SIZE].number_value());
    }

    if(config[BINARY_IMAGE_THRESHOLD].is_number())
    {
        m_config.binaryImageThreshold = static_cast<int>(
            config[BINARY_IMAGE_THRESHOLD].number_value());
    }
}

DatatypeFlags MarkerTracker::depends()
{
    return RGB_CAMERA_FLAG;
}

void MarkerTracker::getPoses(const Datapoint& dp, std::vector<Pose>& output)
{
    using namespace cv;

    Mat binary, gray;
    {
        Mat resizedColored;
        // Downscale and convert to gray scale.
        resize(dp.data, resizedColored, m_config.detectorConfig.trackingResolution);
        cvtColor(resizedColored, gray, CV_BGR2GRAY);

        // Mark areas that are between totally black (0) and gray
        // threshold (default 100) with black, others with white. Tracking
        // and id-detection is done with this Mat.
        inRange(gray, 0, m_config.binaryImageThreshold, binary);
    }

    // TODO: Find markers modifies the binary image,
    // so it should probably be copied.
    auto markers = m_detector->findMarkers(binary);

    for(const auto& marker : markers)
    {
        Mat rectified = m_detector->getRectifiedInnerImage(marker.innerContour, gray, m_config.markerParserImageSize);
        MarkerValue value = m_parser->getData(rectified);
        if(!value.valid) continue;

        Pose cameraPose = m_detector->getPose(value.id, marker.outerContour);
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
            m_detector->drawPolygon(marker.innerContour, dp.data);
            m_detector->drawPolygon(marker.outerContour, dp.data);
            m_detector->drawAxes(dp.data, cameraPose.rotation, cameraPose.translation);
#endif
        }

        output.push_back(cameraPose);
    }

    // Gather all ids, that were used during this run
    // and prune detector history accordingly.
    std::vector<int> usedIds(output.size());
    std::transform(output.begin(), output.end(), usedIds.begin(),
        [](const Pose& pose)
        {
            return pose.coordinateSystemId;
        });
    m_detector->pruneHistory(usedIds);
}
}
