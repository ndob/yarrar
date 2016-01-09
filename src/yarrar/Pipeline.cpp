#include "Pipeline.h"
#include "dataprovider/StaticImageDataProvider.h"
#include "dataprovider/WebcamDataProvider.h"
#include "dataprovider/AndroidImageProvider.h"
#include "tracker/marker/MarkerTracker.h"
#include "renderer/opengl/OpenGLRenderer.h"
#include "renderer/opencv/OpenCVRenderer.h"
#include "renderer/dummy/DummyRenderer.h"

#include <algorithm>

#define VALIDATE_PIPELINE_STAGE_JSON(stageConfig)                  \
    if(!stageConfig.has_shape(PIPELINE_STAGE_SHAPE, err))          \
    {                                                              \
        throw std::runtime_error(std::string("Pipeline: ") + err); \
    }

namespace
{

const json11::Json::shape TOP_LEVEL_SHAPE{
    { "dataproviders", json11::Json::ARRAY },
    { "trackers", json11::Json::ARRAY },
    { "renderers", json11::Json::ARRAY },
};

const json11::Json::shape PIPELINE_STAGE_SHAPE{
    { "type", json11::Json::STRING },
    { "config", json11::Json::OBJECT }
};
}

namespace yarrar
{

Pipeline::Pipeline(const std::string& configFile)
{
    auto config = util::loadJson(configFile);

    std::string err;
    if(!config["pipeline"].has_shape(TOP_LEVEL_SHAPE, err))
    {
        throw std::runtime_error(std::string("Pipeline: ") + err);
    }

    auto pipeline = config["pipeline"];

    auto dataproviders = pipeline["dataproviders"];
    for(const auto& provider : dataproviders.array_items())
    {
        VALIDATE_PIPELINE_STAGE_JSON(provider);
        auto type = provider["type"].string_value();

        if(type == "webcam")
        {
            addDataProvider<WebcamDataProvider>(provider["config"]);
        }
        else if(type == "static_image")
        {
            addDataProvider<StaticImageDataProvider>(provider["config"]);
        }
        else if(type == "android_image")
        {
            addDataProvider<AndroidImageProvider>(provider["config"]);
        }
    }

    auto trackers = pipeline["trackers"];
    for(const auto& tracker : trackers.array_items())
    {
        VALIDATE_PIPELINE_STAGE_JSON(tracker);
        auto type = tracker["type"].string_value();

        if(type == "marker")
        {
            addTracker<MarkerTracker>(tracker["config"]);
        }
    }

    auto renderers = pipeline["renderers"];
    for(const auto& renderer : renderers.array_items())
    {
        VALIDATE_PIPELINE_STAGE_JSON(renderer);
        auto type = renderer["type"].string_value();

        if(type == "opengl")
        {
            addRenderer<OpenGLRenderer>(renderer["config"]);
        }
        else if(type == "opencv")
        {
            addRenderer<OpenCVRenderer>(renderer["config"]);
        }
        else if(type == "dummy")
        {
            addRenderer<DummyRenderer>(renderer["config"]);
        }
    }

    validate();
}

void Pipeline::run() const
{
    auto rawData = m_dataProviders[0]->getData();
    if(rawData.total() == 0) return;

    std::vector<Pose> poses;
    for(const auto& tracker : m_trackers)
    {
        tracker->getPoses(rawData, poses);
    }

    for(const auto& renderer : m_renderers)
    {
        renderer->draw(poses, m_scene, rawData);
    }
}

void Pipeline::addModel(const Model& model)
{
    m_scene.addModel(model);
    for(const auto& renderer : m_renderers)
    {
        renderer->loadModel(model);
    }
}

void Pipeline::validate()
{
    int trackerDependencies = 0;
    int provided = 0;
    for(const auto& tracker : m_trackers)
    {
        trackerDependencies |= tracker->depends();
    }

    for(const auto& provider : m_dataProviders)
    {
        provided |= provider->provides();
    }

    if(trackerDependencies != provided)
    {
        throw std::runtime_error(util::format("Pipeline: Dependency mismatch. needed %#x, got %#x.", trackerDependencies, provided));
    }

    const auto rgbProviders = std::count_if(m_dataProviders.begin(), m_dataProviders.end(),
        [](const std::unique_ptr<DataProvider>& provider)
        {
             return provider->provides() & DatatypeFlags::RGB_CAMERA_FLAG;
        });

    // TODO: This could be relaxed. Instead of throwing, one provider should be promoted as primary.
    // Primary can then be used as backgroundImage-input for Renderer-stage.
    if(rgbProviders > 1)
        throw std::runtime_error("Pipeline: only one rgb-dataprovider should be defined.");
}
}
