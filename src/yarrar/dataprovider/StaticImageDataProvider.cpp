#include "StaticImageDataProvider.h"

namespace {

const json11::Json::shape CONFIG_SHAPE{
    {"image_path", json11::Json::STRING}
};

}

namespace yarrar {

StaticImageDataProvider::StaticImageDataProvider(const json11::Json& config):
    DataProvider(config)
{
    std::string err;
    if(!config.has_shape(CONFIG_SHAPE, err))
    {
        throw std::runtime_error(std::string("StaticImageDataProvider: ") + err);
    }

    auto imagePath = config["image_path"].string_value();
    m_cachedImage = cv::imread(imagePath, cv::IMREAD_COLOR);
    if(m_cachedImage.cols == 0 && m_cachedImage.rows == 0)
    {
        throw std::runtime_error(std::string("StaticImageDataProvider: error reading file") + imagePath);
    }
}

cv::Mat StaticImageDataProvider::getData()
{
    return m_cachedImage;
}

Dimensions StaticImageDataProvider::getDimensions()
{
    return {
        m_cachedImage.cols,
        m_cachedImage.rows
    };
}

DatatypeFlags StaticImageDataProvider::provides()
{
    return RGB_CAMERA_FLAG;
}

}


