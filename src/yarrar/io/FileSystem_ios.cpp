#include "FileSystem.h"
#include "ios/ApplicationPath.h"

#include <stdexcept>
#include <fstream>
#include <sstream>

namespace yarrar
{
namespace io
{

void readFile(const std::string& relativePath, std::string& toBuffer)
{
    std::ifstream file;
    std::string fullPath = iOSGetApplicationPath() + "/data/" + relativePath;
    file.open(fullPath, std::ios::in | std::ios::binary);
    if(!file.is_open())
    {
        throw std::runtime_error(std::string("failed to open file: ") + fullPath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    toBuffer = buffer.str();
}
}
}
