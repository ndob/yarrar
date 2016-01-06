#pragma once

#include <string>

namespace yarrar {
namespace filesystem {

void readFile(const std::string& relativePath, std::string& toBuffer);

}
}
