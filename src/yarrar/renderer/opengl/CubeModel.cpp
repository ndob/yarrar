#include "CubeModel.h"
#include "GLProgram.h"

namespace yarrar
{

CubeModel::CubeModel(std::weak_ptr<GLProgram> program):
    m_vao(0),
    m_vbo(0),
    m_program(program),
    m_numVertices(36)
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    GLfloat vertexData[] = {
            // bottom
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,

            // top
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            // front
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,

            // back
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,

            // left
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,

            // right
            1.0f,-1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f, 1.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    if(auto prog = m_program.lock())
    {
        GLint vertAttrib = prog->getAttrib("vert");
        glEnableVertexAttribArray(vertAttrib);
        glVertexAttribPointer(vertAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    }
    else
    {
        throw std::runtime_error("Program not available.");
    }

    // unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CubeModel::render() const
{
    if(auto prog = m_program.lock())
    {
        ScopedUseProgram _p(prog.get());
        glBindVertexArray(m_vao);

        glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

        glBindVertexArray(0);
    }
    else
    {
        throw std::runtime_error("Program not available.");
    }
}

}
