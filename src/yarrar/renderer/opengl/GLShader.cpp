#include "GLShader.h"
#include "OpenGLRenderer.h"
#include "io/FileSystem.h"

#include <stdexcept>
#include <vector>

namespace yarrar {

GLShader::GLShader(const ShaderDef& def, GLenum shaderType):
    m_object(0)
{
    m_object = glCreateShader(shaderType);
    if(m_object == 0)
    {
        throw std::runtime_error("glCreateShader failed");
    }

    std::string code;
    filesystem::readFile(def.path, code);

    const GLchar* strings[] = { code.c_str() };
    glShaderSource(m_object, 1, strings, nullptr);
    glCompileShader(m_object);

    GLint status;
    glGetShaderiv(m_object, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetShaderiv(m_object, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> logChars(logLength + 1);
        glGetShaderInfoLog(m_object, logLength, nullptr, &logChars.front());

        glDeleteShader(m_object);

        std::string msg("Compile failure in shader:\n");
        msg += std::string(logChars.begin(), logChars.end());
        throw std::runtime_error(msg);
    }
}

}

