#pragma once

#include "GLModel.h"

#include <GL/glew.h>

#include <memory>

namespace yarrar
{

class GLProgram;

class CubeModel: public GLModel
{
public:
    CubeModel(std::weak_ptr<GLProgram> program);
    void render() const override;

private:
    GLuint m_vao;
    GLuint m_vbo;
    std::weak_ptr<GLProgram> m_program;
    int m_numVertices;
};

}

