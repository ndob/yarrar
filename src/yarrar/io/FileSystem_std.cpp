#include "FileSystem.h"

#include <stdexcept>
#include <fstream>
#include <sstream>

// TODO: Implement unified path handling and remove these.
#ifdef _MSC_VER
std::string DATA_PATH = "../../../data/";
#else
std::string DATA_PATH = "../../data/";
#endif

namespace yarrar {
namespace filesystem {

void readFile(const std::string& relativePath, std::string& toBuffer)
{
    std::ifstream file;
    file.open(DATA_PATH + relativePath, std::ios::in | std::ios::binary);
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
