#include "StaticImageDataProvider.h"

namespace
{

const json11::Json::shape CONFIG_SHAPE{
    { "image_path", json11::Json::STRING }
};
}

namespace yarrar
{

StaticImageDataProvider::StaticImageDataProvider(const json11::Json& config)
    : DataProvider(config)
    , m_dp({})
{
    std::string err;
    if(!config.has_shape(CONFIG_SHAPE, err))
    {
        throw std::runtime_error(std::string("StaticImageDataProvider: ") + err);
    }

    auto imagePath = config["image_path"].string_value();
    auto cachedImage = cv::imread(imagePath, cv::IMREAD_COLOR);
    if(cachedImage.cols == 0 && cachedImage.rows == 0)
    {
        throw std::runtime_error(std::string("StaticImageDataProvider: error reading file") + imagePath);
    }

    m_imageDimensions = {
        cachedImage.cols,
        cachedImage.rows
    };

    auto handle = m_dp.lockReadWrite();
    handle.set({ std::chrono::high_resolution_clock::now(),
        cachedImage });
}
const LockableData<Datapoint>& StaticImageDataProvider::getData()
{
    return m_dp;
}

Dimensions StaticImageDataProvider::getDimensions()
{
    return m_imageDimensions;
}

DatatypeFlags StaticImageDataProvider::provides()
{
    return RGB_CAMERA_FLAG;
}
}
