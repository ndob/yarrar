#include "SceneModel.h"
#include "GLProgram.h"

namespace yarrar
{

SceneModel::SceneModel(std::weak_ptr<GLProgram> program, const std::vector<float>& vertices):
    m_vao(0),
    m_vbo(0),
    m_program(program),
    m_numVertices(static_cast<int> (vertices.size() / 3))
{
    // GL3 uses vertex arrays, but they are not available in
    // GLES2.
#ifdef YARRAR_OPENGL_CONTEXT
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
#endif

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int> (vertices.size()) * sizeof(float), &vertices.front(), GL_STATIC_DRAW);

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

void SceneModel::render(const Vector3& translation) const
{
    float world[][4] = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {translation.x, translation.y, translation.z, 1.0f}
    };

    if(auto prog = m_program.lock())
    {
        ScopedUseProgram _p(prog.get());
        prog->setUniformMatrix4fv("world", (GLfloat*) world);

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
