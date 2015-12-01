#pragma once

#include "GLModel.h"

//#include <GL/glew.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


#include <memory>

namespace yarrar
{

class GLProgram;

class BackgroundModel: public GLModel
{
public:
    BackgroundModel(std::weak_ptr<GLProgram> program);
    void render() const override;
    void setTexture(GLuint texture);

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_texture;
    std::weak_ptr<GLProgram> m_program;
    int m_numVertices;
};

}

