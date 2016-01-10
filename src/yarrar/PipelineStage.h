#pragma once

#include "Types.h"
#include "Scene.h"
#include "LockableData.h"

#include <json11.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

namespace yarrar
{

class PipelineStage
{
public:
    PipelineStage(const json11::Json& config){};
    virtual ~PipelineStage(){};
};

class DataProvider : public PipelineStage
{
public:
    DataProvider(const json11::Json& config)
        : PipelineStage(config){};
    virtual const LockableData<Datapoint>& getData() = 0;
    virtual Dimensions getDimensions() = 0;
    virtual DatatypeFlags provides() = 0;
};

class Tracker : public PipelineStage
{
public:
    Tracker(const json11::Json& config)
        : PipelineStage(config){};
    virtual DatatypeFlags depends() = 0;
    virtual void getPoses(const Datapoint& rawData,
        std::vector<Pose>& output) = 0;
};

class Renderer : public PipelineStage
{
public:
    Renderer(const json11::Json& config)
        : PipelineStage(config){};
    virtual void loadModel(const Model& model) = 0;
    virtual void draw(const std::vector<Pose>& cameraPoses,
        const Scene& scene,
        const Datapoint& rawData) = 0;
};
}
