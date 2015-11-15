#pragma once

#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Pipeline.h"
#include "Pose.h"

#include <string>
#include <map>

namespace yarrar {

class GLShader;
class GLProgram;
class BackgroundModel;
class CubeModel;

struct EffectDef
{
    std::string name;
    std::string vertexShader;
    std::string fragmentShader;
    bool perspectiveProjection;
};

struct ShaderDef
{
    std::string name;
    std::string path;
};

class OpenGLRenderer: public Renderer
{
public:
    OpenGLRenderer(int width, int height);
    ~OpenGLRenderer();
    OpenGLRenderer(const OpenGLRenderer&) = delete;
    OpenGLRenderer operator=(const OpenGLRenderer&) = delete;
    OpenGLRenderer(const OpenGLRenderer&&) = delete;
    OpenGLRenderer operator=(const OpenGLRenderer&&) = delete;

    void draw(const Pose& cameraPose, const cv::Mat& image) override;
private:
    void initializeGLFW();
    void initializeGLEW();
    void render();
    void loadImage(const cv::Mat& image);

    std::map<std::string, std::unique_ptr<GLShader>> m_vertexShaders;
    std::map<std::string, std::unique_ptr<GLShader>> m_fragmentShaders;
    std::map<std::string, std::shared_ptr<GLProgram>> m_programs;

    GLFWwindow* m_window;
    std::unique_ptr<BackgroundModel> m_bgModel;
    std::unique_ptr<CubeModel> m_cubeModel;
    GLuint m_backgroundTex;

    int m_screenWidth;
    int m_screenHeight;
};

}

