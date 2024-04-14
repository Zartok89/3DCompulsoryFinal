#include "Scene.h"
#include <imgui.h>
#include "camera/Camera.h"
#include "utility/Logger.h"

Scene::Scene(const std::string& name)
{

}

Scene::~Scene()
{
}

void Scene::LoadContent()
{
	LOG_INFO("Scene::LoadContent");

}

void Scene::UnloadContent()
{
	LOG_INFO("Scene::UnloadContent");

}

void Scene::UpdatingScene(float dt)
{
}

void Scene::RenderingScene(float dt)
{
	static bool bShowDemoWindow = true;
	if (bShowDemoWindow)
	{ 
		ImGui::ShowDemoWindow(&bShowDemoWindow);
	}

	glEnable(GL_DEPTH_TEST);
}

void Scene::FramebufferSizeCallback(Window* window, int width, int height)
{
	//mSceneCamera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

void Scene::MouseMoveCallback(Window* window, double xpos, double ypos)
{
}

void Scene::MouseButtonCallback(Window* window, int button, int action, int mods)
{
}

void Scene::MouseScrollCallback(Window* window, double xoffset, double yoffset)
{
}

void Scene::KeyCallback(Window* window, int key, int scancode, int action, int mods)
{
}

