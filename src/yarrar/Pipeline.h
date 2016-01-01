#pragma once

#include "Pose.h"
#include "Scene.h"

#include <memory>
#include <vector>
#include <opencv2/core/mat.hpp>

namespace {

const int PREFERRED_TRACKING_RESOLUTION_WIDTH = 320;

}

namespace yarrar {

struct Dimensions
{
    int width;
    int height;
};

class DataProvider
{
public:
    virtual ~DataProvider() {};
    virtual cv::Mat getData() = 0;
    virtual Dimensions getDimensions() = 0;
};

class Tracker
{
public:
    virtual ~Tracker() {};
    virtual Pose getPose(const cv::Mat& rawData) = 0;
};

class Renderer
{
public:
    virtual ~Renderer() {};
    virtual void loadModel(const Model& model) = 0;
    virtual void draw(const Pose& cameraPose,
                      const Scene& scene,
                      const cv::Mat& backgroundImage) = 0;
};

class Pipeline
{
public:

    template<
        typename T,
        typename... Args
    >
    void addDataProvider(Args&&... args)
    {
        m_dataProviders.emplace_back(new T(std::forward<Args>(args)...));
    }

    template<typename T>
    void addTracker()
    {
        assert(m_dataProviders.size() == 1);
        auto dim = m_dataProviders[0]->getDimensions();
        cv::Size trackingRes = getScaledDownResolution(dim.width,
                                                       dim.height,
                                                       PREFERRED_TRACKING_RESOLUTION_WIDTH);
        m_trackers.emplace_back(new T(trackingRes));
    }

    template<typename T>
    void addRenderer()
    {
        assert(m_dataProviders.size() == 1);
        auto dim = m_dataProviders[0]->getDimensions();
        m_renderers.emplace_back(new T(dim.width, dim.height));
    }

    void addModel(const Model& model);

    void run() const;

private:
    std::vector<std::unique_ptr<DataProvider>> m_dataProviders;
    std::vector<std::unique_ptr<Tracker>> m_trackers;
    std::vector<std::unique_ptr<Renderer>> m_renderers;
    Scene m_scene;
};

}
