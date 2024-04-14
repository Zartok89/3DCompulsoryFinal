#pragma once

#include "Window.h"
#include "actors/Scene.h"

class Application
{
    public:
    Application(const Application&) = delete; // Deleting the copy constructor
    Application(Application&&) = delete; // Deleting the move constructor

    static Application* GetApplication(); // Creating a single instance of Application

    void InitializingApplication(); // Initializing the Application with the required functions

    void InitializeGLFW(); // Initializing the GLFW setup

    void LoadProjectContent(); // Loading content from the Window class, which loads the content of the Scene

    void Run(); // Running the application with all its content and functoinality
    
private:
    Application() = default; // Default constructor

    Scene mScene{"Scene"}; // Default scene class object
    Window mWindow{"FinalCompulsory", &mScene, 1920, 1080}; // Default window class object

};