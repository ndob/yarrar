#include "Pipeline.h"
#include "dataprovider/StaticImageDataProvider.h"
#include "dataprovider/WebcamDataProvider.h"
#ifdef ANDROID
#include "dataprovider/AndroidCameraProvider.h"
#include "dataprovider/AndroidGyroscopeProvider.h"
#include "dataprovider/AndroidSensorProvider.h"
#endif
#include "tracker/marker/MarkerTracker.h"
#include "tracker/sensor/SensorTracker.h"
#include "fusion/vsfusion/VisualWithSensors.h"
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
#ifdef ANDROID
        else if(type == "android_image")
        {
            addDataProvider<AndroidCameraProvider>(provider["config"]);
        }
        else if(type == "android_sensor")
        {
            addDataProvider<AndroidSensorProvider>(provider["config"]);
        }
        else if(type == "android_gyro")
        {
            addDataProvider<AndroidGyroscopeProvider>(provider["config"]);
        }
#endif
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
        else if(type == "sensor")
        {
            addTracker<SensorTracker>(tracker["config"]);
        }
    }

    auto fusions = pipeline["fusion"];
    for(const auto& fusion : fusions.array_items())
    {
        VALIDATE_PIPELINE_STAGE_JSON(fusion);
        auto type = fusion["type"].string_value();

        if(type == "visual_with_sensors")
        {
            addSensorFusion<VisualWithSensors>(fusion["config"]);
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
    // Camera data is handled separately and is always in index 0.
    // TODO: Remove hard coded index. Separate visual and sensor data inputs?
    const auto& rawCameraData = m_dataProviders[0]->getData();
    auto readLock = rawCameraData.lockRead();
    const auto& cameraData = readLock.get();
    if(cameraData.data.total() == 0) return;

    std::vector<Pose> poses;

    if(m_sensorFusions.empty())
    {
        for(const auto& tracker : m_trackers)
        {
            tracker->getPoses(cameraData, poses);
        }
    }
    else
    {
        // Other (sensor) dataproviders will be used in fusion stage.
        std::vector<std::reference_wrapper<const LockableData<Datapoint>>> datapoints;
        for(size_t i = 1; i < m_dataProviders.size(); ++i)
        {
            datapoints.push_back(std::cref(m_dataProviders[i]->getData()));
        }

        std::map<size_t, std::vector<Pose>> posesByTrackerIndex;
        for(size_t i = 0; i < m_trackers.size(); ++i)
        {
            m_trackers[i]->getPoses(cameraData, posesByTrackerIndex[i]);
        }

        for(const auto& fusion : m_sensorFusions)
        {
            fusion->getFusedPoses(datapoints, posesByTrackerIndex, poses);
        }
    }

    for(const auto& renderer : m_renderers)
    {
        renderer->draw(poses, m_scene, cameraData);
    }
}

void Pipeline::addModel(int coordinateSystemId, const Model& model)
{
    m_scene.addModel(coordinateSystemId, model);
    for(const auto& renderer : m_renderers)
    {
        renderer->loadModel(model);
    }
}

void Pipeline::validate()
{
    int32_t trackerDependencies = 0;
    int32_t provided = 0;
    for(const auto& tracker : m_trackers)
    {
        trackerDependencies |= tracker->depends();
    }

    for(const auto& provider : m_dataProviders)
    {
        provided |= provider->provides();
    }

    if((trackerDependencies & provided) != trackerDependencies)
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
