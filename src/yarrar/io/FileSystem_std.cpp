#include "FileSystem.h"
#include "ApplicationPath.h"

#include <stdexcept>
#include <fstream>
#include <sstream>

// TODO: Implement unified path handling and remove these.
#ifdef _MSC_VER
std::string DATA_PATH = "../../../data/";
#else
std::string DATA_PATH = "../../data/";
#endif

namespace yarrar
{
namespace io
{

void readFile(const std::string& relativePath, std::string& toBuffer)
{
    // TODO: APPLE
    std::ifstream file;
    DATA_PATH = getPath();
    std::string fullPath = DATA_PATH + "/" + relativePath;
    file.open(fullPath, std::ios::in | std::ios::binary);
    if(!file.is_open())
    {
        throw std::runtime_error(std::string("failed to open file: ") + relativePath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    toBuffer = buffer.str();
}
}
}
