#pragma once

#ifdef YARRAR_OPENGL_CONTEXT
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#elif YARRAR_OPENGLES_CONTEXT
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#endif
#endif

namespace yarrar
{

class GLContext
{
public:
    GLContext(int width, int height);
    ~GLContext();

    void swapBuffers();

private:
#ifdef YARRAR_OPENGL_CONTEXT
    GLFWwindow* m_window;
#endif

    int m_screenWidth;
    int m_screenHeight;
};
}
