#include "FileSystem.h"
#include "android/AndroidServices.h"

#include <stdexcept>
#include <fstream>
#include <sstream>

namespace
{

class AndroidFile
{
public:
    AndroidFile(const std::string& path)
        : m_path(path)
        , m_file(nullptr)
    {
        m_file = AAssetManager_open(yarrar::android::getAssetManager(), m_path.c_str(), AASSET_MODE_BUFFER);
        if(!m_file)
        {
            throw std::runtime_error(std::string("failed to open file: ") + m_path);
        }
    }

    ~AndroidFile()
    {
        AAsset_close(m_file);
    }

    void read(std::string& toBuffer)
    {
        if(!m_file)
        {
            throw std::runtime_error(std::string("file not opened: ") + m_path);
        }

        size_t len = AAsset_getLength(m_file);
        toBuffer.clear();
        toBuffer.resize(len);

        int ret = AAsset_read(m_file, &toBuffer.front(), len);
        if(ret <= 0)
        {
            throw std::runtime_error(std::string("read error in file: ") + m_path);
        }
    }

private:
    std::string m_path;
    AAsset* m_file;
};
}


namespace yarrar
{
namespace io
{

void readFile(const std::string& relativePath, std::string& toBuffer)
{
    yarrar::android::log(std::string("Opening file: ") + relativePath);
    AndroidFile f(relativePath);
    f.read(toBuffer);
}
}
}
