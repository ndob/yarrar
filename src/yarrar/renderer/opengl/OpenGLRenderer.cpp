#include "OpenGLRenderer.h"
#include "GLProgram.h"
#include "GLShader.h"
#include "BackgroundModel.h"
#include "CubeModel.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define YARRAR_OPENGL_CONTEXT

namespace {

    using namespace yarrar;

    const std::vector<ShaderDef> VERTEX_SHADERS
    {
       {"camera", "../../data/shader/camera.vertex"},
       {"simple", "../../data/shader/simple.vertex"}
    };

    const std::vector<ShaderDef> FRAGMENT_SHADERS
    {
        {"simple", "../../data/shader/simple.fragment"},
        {"texture", "../../data/shader/texture.fragment"}
    };

    const std::vector<EffectDef> EFFECTS
    {
        {"cube", "camera", "simple", true},
        {"bg", "simple", "texture", false}
    };

}

namespace yarrar {

OpenGLRenderer::OpenGLRenderer(int width, int height):
    m_backgroundTex(0),
    m_screenWidth(width),
    m_screenHeight(height)
{
    initializeGLFW();
    initializeGLEW();

    for(const auto& def : VERTEX_SHADERS)
    {
        m_vertexShaders[def.name] = std::unique_ptr<GLShader>(new GLShader(def, GL_VERTEX_SHADER));
    }

    for(const auto& def : FRAGMENT_SHADERS)
    {
        m_fragmentShaders[def.name] = std::unique_ptr<GLShader>(new GLShader(def, GL_FRAGMENT_SHADER));
    }

    glm::mat4 projection = glm::perspective(2.0f, static_cast<float> (m_screenWidth) / static_cast<float> (m_screenHeight), 0.1f, 10.0f);

    for(const auto& def : EFFECTS)
    {
        m_programs[def.name] =
                std::shared_ptr<GLProgram>(new GLProgram(
                                                   m_vertexShaders[def.vertexShader].get(),
                                                   m_fragmentShaders[def.fragmentShader].get(),
                                                   def.perspectiveProjection));
        if(def.perspectiveProjection)
        {
            m_programs[def.name]->setUniformMatrix4fv("projection", glm::value_ptr(projection));
        }
    }

    m_bgModel.reset(new BackgroundModel(m_programs["bg"]));
    m_cubeModel.reset(new CubeModel(m_programs["cube"]));

    glGenTextures(1, &m_backgroundTex);
}

OpenGLRenderer::~OpenGLRenderer()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void OpenGLRenderer::initializeGLFW()
{
    if(!glfwInit())
    {
        throw std::runtime_error("glfwInit failed");
    }

#if defined(YARRAR_OPENGL_CONTEXT)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#elif defined(YARRAR_OPENGL_ES_CONTEXT)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DECORATED, GL_TRUE);

    m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, "yarrarr", nullptr, nullptr);
    if(!m_window)
    {
        throw std::runtime_error("glfwCreateWindow failed");
    }

    glfwMakeContextCurrent(m_window);

    std::cout << "GLFW initialized successfully. " << std::endl;
}

void OpenGLRenderer::initializeGLEW()
{
    if(glewInit() != GLEW_OK)
    {
        throw std::runtime_error("glewInit failed");
    }

    if(!GLEW_VERSION_3_2)
    {
        throw std::runtime_error("OpenGL 3.2 required, but not found.");
    }

    std::cout << "GLEW initialized successfully with params:" << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGLRenderer: " << glGetString(GL_RENDERER) << std::endl;
}

void OpenGLRenderer::render()
{
    // Clear screen.
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Disable depth buffering for background rendering.
    glDisable(GL_DEPTH_TEST);
    m_bgModel->setTexture(m_backgroundTex);
    m_bgModel->render();

    // Depth buffering back on for 3D models.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    m_cubeModel->render();

    glfwSwapBuffers(m_window);
}

void OpenGLRenderer::loadImage(const cv::Mat& image)
{
    // OpenCV stores images flipped in in comparison to OpenGL.
    cv::Mat flipped;
    cv::flip(image, flipped, 0);
    glBindTexture(GL_TEXTURE_2D, m_backgroundTex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D,
                 0, // Mip level
                 GL_RGB,
                 flipped.cols, // Width
                 flipped.rows, // Height
                 0, // Border
                 GL_BGR, // Input format: OpenCV is using BGR.
                 GL_UNSIGNED_BYTE, // Data type
                 flipped.ptr());
}

void OpenGLRenderer::draw(const yarrar::Pose &cameraPose, const cv::Mat& image)
{
    if(cameraPose.valid)
    {
        glfwPollEvents();

        // Convert rotation from vector to matrix.
        cv::Mat rotation;
        cv::Rodrigues(cameraPose.rotation, rotation);

        cv::Mat viewMatrix(cv::Mat::zeros(4, 4, CV_32F));
        for(int row = 0; row < 3; ++row)
        {
            for(int col = 0; col < 3; ++col)
            {
                viewMatrix.at<float>(row, col) = (float) rotation.at<double>(row, col);
            }
            viewMatrix.at<float>(row, 3) = (float) cameraPose.translation.at<double>(row, 0);
        }
        viewMatrix.at<float>(3, 3) = 1.0f;

        cv::Mat openCVToOpenGl = cv::Mat::zeros(4, 4, CV_32F);
        openCVToOpenGl.at<float>(0, 0) = 1.0f;
        openCVToOpenGl.at<float>(1, 1) = -1.0f; // Invert the y axis
        openCVToOpenGl.at<float>(2, 2) = -1.0f; // invert the z axis
        openCVToOpenGl.at<float>(3, 3) = 1.0f;

        viewMatrix = openCVToOpenGl * viewMatrix;
        cv::Mat glViewMatrix = cv::Mat::zeros(4, 4, CV_32F);

        // OpenCV is row major, OpenGL column major.
        cv::transpose(viewMatrix, glViewMatrix);

        for(const auto& program : m_programs)
        {
            if(program.second->getUsePerspectiveProjection())
            {
                ScopedUseProgram p_(program.second.get());
                program.second->setUniformMatrix4fv("camera", (GLfloat *) glViewMatrix.data);
            }
        }
    }

    // Load fresh background camera image.
    loadImage(image);
    render();
}

}