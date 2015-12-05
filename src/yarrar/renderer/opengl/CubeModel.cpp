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
#ifdef YARRAR_OPENGL_CONTEXT
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
#endif

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    GLfloat vertexData[] = {

            // bottom
            -1.0f,  -1.0f,  0.0f,
            1.0f,   -1.0f,  0.0f,
            -1.0f,  -1.0f,  2.0f,
            1.0f,   -1.0f,  0.0f,
            1.0f,   -1.0f,  2.0f,
            -1.0f,  -1.0f,  2.0f,

            // top
            -1.0f,  1.0f,   0.0f,
            -1.0f,  1.0f,   2.0f,
            1.0f,   1.0f,   0.0f,
            1.0f,   1.0f,   0.0f,
            -1.0f,  1.0f,   2.0f,
            1.0f,   1.0f,   2.0f,

            // front
            -1.0f,  -1.0f,  2.0f,
            1.0f,   -1.0f,  2.0f,
            -1.0f,  1.0f,   2.0f,
            1.0f,   -1.0f,  2.0f,
            1.0f,   1.0f,   2.0f,
            -1.0f,  1.0f,   2.0f,

            // back
            -1.0f,  -1.0f,  0.0f,
            -1.0f,  1.0f,   0.0f,
            1.0f,   -1.0f,  0.0f,
            1.0f,   -1.0f,  0.0f,
            -1.0f,  1.0f,   0.0f,
            1.0f,   1.0f,   0.0f,

            // left
            -1.0f,  -1.0f,  2.0f,
            -1.0f,  1.0f,   0.0f,
            -1.0f,  -1.0f,  0.0f,
            -1.0f,  -1.0f,  2.0f,
            -1.0f,  1.0f,   2.0f,
            -1.0f,  1.0f,   0.0f,

            // right
            1.0f,   -1.0f,  2.0f,
            1.0f,   -1.0f,  0.0f,
            1.0f,   1.0f,   0.0f,
            1.0f,   -1.0f,  2.0f,
            1.0f,   1.0f,   0.0f,
            1.0f,   1.0f,   2.0

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
#ifdef YARRAR_OPENGL_CONTEXT
    glBindVertexArray(0);
#endif
}

void CubeModel::render() const
{
    if(auto prog = m_program.lock())
    {
        ScopedUseProgram _p(prog.get());
#ifdef YARRAR_OPENGL_CONTEXT
        glBindVertexArray(m_vao);
#elif YARRAR_OPENGLES_CONTEXT
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
#endif

        glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

#ifdef YARRAR_OPENGL_CONTEXT
        glBindVertexArray(0);
#elif YARRAR_OPENGLES_CONTEXT
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
    }
    else
    {
        throw std::runtime_error("Program not available.");
    }
}

}
