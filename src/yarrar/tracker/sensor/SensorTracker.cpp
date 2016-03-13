#include "SensorTracker.h"


namespace yarrar
{

SensorTracker::SensorTracker(int width, int height, const json11::Json& config)
    : Tracker(config)
{
}

DatatypeFlags SensorTracker::depends()
{
    return ORIENTATION_SENSOR_FLAG;
}

void SensorTracker::getPoses(const Datapoint& data, std::vector<Pose>& output)
{
    Pose p;
    p.rotation = data.data;
    p.coordinateSystemId = -1;
    output.push_back(p);
}
}
