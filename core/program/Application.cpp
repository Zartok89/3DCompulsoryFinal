#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utility/Logger.h"


Application* Application::GetApplication()
{
    static Application* application = new Application;
    return application;
}

void Application::InitializingApplication()
{
	LOG_INFO("Application::InitializingApplication");
    InitializeGLFW();
    mWindow.InitializingWindow();
    mWindow.RegisterCallback();
}

void Application::InitializeGLFW()
{
	LOG_INFO("Application::InitializeGLFW");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Application::LoadProjectContent()
{
	LOG_INFO("Application::LoadContent");
    mWindow.LoadContent(); 
}

void Application::Run()
{
	InitializingApplication();
    LoadProjectContent();

    float lastFrame = 0.f;

    while (!mWindow.IsClosed())
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        mWindow.StartFrame();
        mWindow.Update(deltaTime); 
        mWindow.Render(deltaTime);
        mWindow.EndFrame();
    }

    glfwTerminate();
}
