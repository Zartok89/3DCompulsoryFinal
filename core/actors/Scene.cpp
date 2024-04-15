#include "Scene.h"
#include <imgui.h>
#include "camera/Camera.h"
#include "glad/glad.h"
#include "utility/Logger.h"

Scene::Scene(const std::string& name)
{
	mSceneGraph = name;
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

void Scene::UpdateSceneGraph(Actor* actor, float dt, Transform globalTransform)
{
	if (!actor) return;

	globalTransform.SetTransformMatrix(globalTransform.GetTransformMatrix() * actor->GetTransformMatrix());

	actor->Update(dt);

	const auto& children = actor->GetChildren();
	for (Actor* child : children) 
	{
		UpdateSceneGraph(child, dt, globalTransform);
	}
}

void Scene::RenderSceneGraph(Actor* actor, float dt, Transform globalTransform)
{
	if (!actor) return;

	globalTransform.SetTransformMatrix(globalTransform.GetTransformMatrix() * actor->GetTransformMatrix());

	const auto& children = actor->GetChildren();
	for (Actor* child : children)
	{
		RenderSceneGraph(child, dt, globalTransform);
	}
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

