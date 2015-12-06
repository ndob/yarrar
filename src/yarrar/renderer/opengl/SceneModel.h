#pragma once

#include "GLContext.h"
#include "Util.h"

#include <memory>
#include <vector>

namespace yarrar
{

class GLProgram;

class SceneModel
{
public:
    SceneModel(std::weak_ptr<GLProgram> program, const std::vector<float>& vertices);
    void render(const Vector3& translation) const;

private:
    GLuint m_vao;
    GLuint m_vbo;
    std::weak_ptr<GLProgram> m_program;
    int m_numVertices;
};

}

