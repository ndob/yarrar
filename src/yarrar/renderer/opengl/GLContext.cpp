#include "GLContext.h"

#include <iostream>
#include <stdexcept>

namespace yarrar
{

GLContext::GLContext(int width, int height)
    : m_screenWidth(width)
    , m_screenHeight(height)
{
#ifdef YARRAR_OPENGL_CONTEXT
    if(!glfwInit())
    {
        throw std::runtime_error("glfwInit failed");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);

    m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, "yarrarr", nullptr, nullptr);
    if(!m_window)
    {
        throw std::runtime_error("glfwCreateWindow failed");
    }

    glfwMakeContextCurrent(m_window);

    std::cout << "GLFW initialized successfully. Resolution: "
              << m_screenWidth << "x" << m_screenHeight << std::endl;


    if(glewInit() != GLEW_OK)
    {
        throw std::runtime_error("glewInit failed");
    }

#elif YARRAR_OPENGLES_CONTEXT
// EGL initialize
#endif

    std::cout << "OpenGL initialized successfully with params:" << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGLRenderer: " << glGetString(GL_RENDERER) << std::endl;
}

GLContext::~GLContext()
{
#ifdef YARRAR_OPENGL_CONTEXT
    glfwDestroyWindow(m_window);
    glfwTerminate();
#endif
}

void GLContext::swapBuffers()
{
#ifdef YARRAR_OPENGL_CONTEXT
    glfwSwapBuffers(m_window);
#endif
}
}
