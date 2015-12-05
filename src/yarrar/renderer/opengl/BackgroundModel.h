#pragma once

#include "GLModel.h"
#include "GLContext.h"

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

