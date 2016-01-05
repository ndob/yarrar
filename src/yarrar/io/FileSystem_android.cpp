#include "FileSystem.h"
#include "android/AndroidServices.h"

#include <stdexcept>
#include <fstream>
#include <sstream>

namespace yarrar {
namespace filesystem {

void read(const std::string& relativePath, std::string& toBuffer)
{
    auto assetManager = yarrar::android::getAssetManager();
    AAsset* file = AAssetManager_open(assetManager, relativePath.c_str(), AASSET_MODE_BUFFER);
    if(!file)
    {
        throw std::runtime_error(std::string("failed to open file: ") + relativePath);
    }

    size_t len = AAsset_getLength(file);
    toBuffer.clear();
    toBuffer.resize(len);

    int ret = AAsset_read(file, &toBuffer.front(), len);
    if(ret <= 0)
    {
        throw std::runtime_error(std::string("failed to open file: ") + relativePath);
    }

    AAsset_close(file);
}

}
}