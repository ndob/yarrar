#include "FileSystem.h"

#include <stdexcept>
#include <fstream>
#include <sstream>

namespace yarrar {
namespace filesystem {

void read(const std::string& relativePath, std::string& toBuffer)
{
    std::ifstream file;
    file.open(relativePath, std::ios::in | std::ios::binary);
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