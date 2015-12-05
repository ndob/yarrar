#pragma once

#include "GLContext.h"

#include <string>

namespace yarrar
{
class GLShader;

class GLProgram
{
public:
    GLProgram(GLShader* vertexShader, GLShader* fragmentShader, bool usePerspectiveProjection);

    void setUniformMatrix4fv(const std::string& name, GLfloat* data);

    GLint getAttrib(const std::string& name) const;
    GLint getUniform(const std::string& name) const;
    GLuint getObject() const
    {
        return m_object;
    };

    bool getUsePerspectiveProjection() const
    {
        return m_usePerspectiveProjection;
    }

private:
    GLuint m_object;
    bool m_usePerspectiveProjection;
};

class ScopedUseProgram
{
public:
    ScopedUseProgram(const GLProgram* object)
    {
        glUseProgram(object->getObject());
    }

    ~ScopedUseProgram()
    {
        glUseProgram(0);
    }
};

}