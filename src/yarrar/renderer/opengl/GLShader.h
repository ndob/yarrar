#pragma once

#include "GLContext.h"

namespace yarrar {

struct ShaderDef;

class GLShader
{
public:
    GLShader(const ShaderDef& def, GLenum shaderType);
    GLuint getObject() const 
    {
        return m_object; 
    };

private:
    GLuint m_object;
};

class ScopedAttachShader
{
public:
    ScopedAttachShader(GLuint program, GLuint shader):
        m_program(program),
        m_shader(shader)
    {
        glAttachShader(m_program, m_shader);
    }

    ~ScopedAttachShader()
    {
        glDetachShader(m_program, m_shader);
    }

private:
    GLuint m_program;
    GLuint m_shader;
};

}
