#pragma once
#include <string>
#include <imgui_impl_glfw.h>
#include "actors/Scene.h"

class Window
{
public:
    Window(std::string name, Scene* scene, int width, int height); // Default constructor of Window
    ~Window(); // Deconstructing window and calls these for GLFW and ImGui as well

    /*
     * Initializing all function and contents before rendering
     */
    void InitializingWindow(); // Creating the actual window
    void RegisterCallback(); // Creating local lambda functions to register the callbaks to the OS
    bool LoadContent(); // Check whether the scene content is loaded or not
	void SetScene(Scene* scene); // Setting which scene is the current scene operating

    /*
     * The GLFW while loop sequence
     */
	void StartFrame(); // What should happend at the first frame
    void Update(float dt); // Updates all the scene object based on deltaTime
    void Render(float dt); // Renders all the scene object based on deltaTime
    void EndFrame(); // What should happend at the last frame of the while cycle

    /*
     * After the GLFW while loop sequence
     */
	bool IsClosed(); // Whether the while loop sequence should close or not based on the active window 


    /*
     * Callback functions which goes through the scene then to the controller interface
     */
	void FramebufferSizeCallback(class GLFWwindow* window, int width, int height); // Callback for the current window size
    void MouseMoveCallback(class GLFWwindow* window, double xpos, double ypos); // Callback for the mouse position
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods); // Callback for the mouse clicks
    void MouseScrollCallback(class GLFWwindow* window, double xoffset, double yoffset); // Callback for the mouse scroll
    void KeyCallback(class GLFWwindow* window, int key, int scancode, int action, int mods); // Callback for the keyboard keys

    /*
     * Setter function related to the window class
     */
    void SetWidth(int width) { mWidth = width; } // Setting the width of the window
    void SetHeight(int height) { mHeight = height; } // Setting the height of the window

    /*
     * Getter function related to the window class
     */
    int GetWidth() const { return mWidth; } // Getting the width of the window
    int GetHeight() const { return mHeight; } // Getting the height of the window
    class GLFWwindow* GetGLFWWindow() const { return mGLFWWindow; }; // Getting the current GLFW window

    /*
     * Member variables
     */
    std::string mName; // Member function name for the window
    int mWidth{1920}, mHeight{1080}; // Member function int for size
    
    Scene* mScene{nullptr}; // Member class for the scene object
    class GLFWwindow* mGLFWWindow{ nullptr }; // Member class for the GLFW window
};
