#pragma once

#include "Pipeline.h"
#include "MarkerDetector.h"
#include "YarrarMarkerParser.h"
#include "Util.h"
#include "Types.h"

#define DEBUG_DRAWING 1

namespace yarrar
{

class MarkerTracker : public Tracker
{
public:
    MarkerTracker(int width, int height, const json11::Json& config);

    DatatypeFlags depends() override;
    void getPoses(const Datapoint& data, std::vector<Pose>& output) override;

private:
    void applyJsonConfig(const json11::Json& config, int width, int height);

    struct Config
    {
        MarkerDetector::Config detectorConfig;
        int markerParserImageSize;
        int binaryImageThreshold;
    };

    Config m_config;
    std::unique_ptr<MarkerDetector> m_detector;
    std::unique_ptr<MarkerParser> m_parser;
};
}
