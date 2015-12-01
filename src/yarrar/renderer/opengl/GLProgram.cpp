#include "GLProgram.h"
#include "GLShader.h"

namespace yarrar
{

GLProgram::GLProgram(GLShader* vertexShader,
                     GLShader* fragmentShader,
                     bool usePerspectiveProjection):
    m_object(0),
    m_usePerspectiveProjection(usePerspectiveProjection)
{
    if(vertexShader == nullptr || fragmentShader == nullptr)
    {
        throw std::runtime_error("shaders not initialized correctly");
    }

    m_object = glCreateProgram();
    if(m_object == 0)
    {
        throw std::runtime_error("glCreateProgram failed");
    }

    // Take vertex and fragment shaders into use.
    {
        ScopedAttachShader _v(m_object, vertexShader->getObject());
        ScopedAttachShader _f(m_object, fragmentShader->getObject());
        glLinkProgram(m_object);
    }

    // Check for errors in linking.
    GLint status;
    glGetProgramiv(m_object, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetProgramiv(m_object, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> logChars(logLength + 1);
        glGetProgramInfoLog(m_object, logLength, nullptr, &logChars.front());

        glDeleteProgram(m_object);

        std::string msg("Program linking failure: ");
        msg += std::string(logChars.begin(), logChars.end());
        throw std::runtime_error(msg);
    }
}


void GLProgram::setUniformMatrix4fv(const std::string& name, GLfloat *data)
{
    ScopedUseProgram _p(this);
    glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, data);
}

GLint GLProgram::getAttrib(const std::string& name) const
{
    if(name.empty())
    {
        throw std::runtime_error("attrib name is empty.");
    }

    GLint attrib = glGetAttribLocation(m_object, name.c_str());

    if(attrib == -1)
    {
        throw std::runtime_error(std::string("Program attrib not found: ") + name);
    }

    return attrib;
}

GLint GLProgram::getUniform(const std::string& name) const
{
    if(name.empty())
    {
        throw std::runtime_error("uniform name is empty.");
    }

    GLint uniform = glGetUniformLocation(m_object, name.c_str());

    if(uniform == -1)
    {
        throw std::runtime_error(std::string("Program uniform not found: ") + name);
    }

    return uniform;
}

}
