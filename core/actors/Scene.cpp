#include "Scene.h"

#include <imgui.h>

#include "camera/CameraController.h"
#include "shaders/Shader.h"
#include "utility/Defines.h"
#include "utility/Logger.h"
#include "Interfaces/RenderInterface.h"
#include "actors/Actor.h"
#include "graphics/Skybox.h"
#include "utility/AssimpLoader.h"

Scene::Scene(const std::string& name) : mSceneGraph(name) {}

Scene::~Scene()
{
}

void Scene::MeshActorLoading(Material* mat)
{
	mCube0 = new PawnActor("Cube0", Mesh::CreateCube(mat));
	mCube1 = new PawnActor("Cube1", Mesh::CreateCube(mat));
	mCube2Player = new PawnActor("mCube2Player", Mesh::CreateCube(mat));
	mSkybox = new Skybox({
		SOURCE_DIRECTORY + "assets/textures/skybox/Starfield_And_Haze_left.png",
		SOURCE_DIRECTORY + "assets/textures/skybox/Starfield_And_Haze_right.png",
		SOURCE_DIRECTORY + "assets/textures/skybox/Starfield_And_Haze_up.png",
		SOURCE_DIRECTORY + "assets/textures/skybox/Starfield_And_Haze_down.png",
		SOURCE_DIRECTORY + "assets/textures/skybox/Starfield_And_Haze_front.png",
		SOURCE_DIRECTORY + "assets/textures/skybox/Starfield_And_Haze_back.png",
		});
	mStaticMeshActor0 = new StaticMeshActor("mStaticMeshActor0");
	AssimpLoader::Load(SOURCE_DIRECTORY + "Assets/Models/Horse.fbx", mStaticMeshActor0);
}

void Scene::LightingActorLoading()
{
	mDirectionalLight = new DirectionalLight("Directional light");
	mPointLight = new PointLight("Point light 0");
}

void Scene::ActorHierarchyLoading()
{
	mSceneGraph.AddChild(&mSceneCamera);
	mSceneGraph.AddChild(mCube0);
	mCube0->AddChild(mCube1);
	mSceneGraph.AddChild(mCube2Player);
	mSceneGraph.AddChild(mPointLight);
	mCube2Player->AddChild(mPointLight);
	mSceneGraph.AddChild(mStaticMeshActor0);
	mSceneGraph.AddChild(mDirectionalLight);
}

void Scene::ActorPositionCollisionLoading()
{
	mCube0->SetPosition({ -2.f, 0.f, 0.f }, Actor::TransformSpace::Global);
	mCube1->SetPosition({ 2.f, 0.f, 0.f }, Actor::TransformSpace::Global);
	mStaticMeshActor0->SetScale(glm::vec3(0.005f));
	mStaticMeshActor0->SetRotation(glm::angleAxis((glm::radians(180.f)), glm::vec3(0.f, 1.f, 0.f)));
	mStaticMeshActor0->SetPosition({0.f, 0.f, 5.f}, Actor::TransformSpace::Global);
	mCube2Player->ChooseCollisionType(2);
	mDirectionalLight->SetLightRotation(-90.f, 1, 0, 0);
}

void Scene::CameraAndControllerLoading()
{
	mSceneCamera.SetPosition({ 0.f, 0.f, 10.f });
	mActorController = std::make_shared<ActorController>(mStaticMeshActor0);
	mCameraController = std::make_shared<CameraController>(&mSceneCamera);
	mCurrentController = mCameraController;
}

void Scene::MaterialTextureLoading(Material*& material)
{
	Texture* diffuseTexture = Texture::Load(SOURCE_DIRECTORY + "assets/textures/container2.jpg");
	Texture* specularTexture = Texture::Load(SOURCE_DIRECTORY + "assets/textures/container2_specular.jpg");
	material = Material::Load("Default", { diffuseTexture, specularTexture }, {});
}

void Scene::LoadContent()
{
	LOG_INFO("Scene::LoadContent");
	Material* material;
	MaterialTextureLoading(material);
	MeshActorLoading(material);
	LightingActorLoading();

	mShader = new Shader(SOURCE_DIRECTORY + "core/shaders/shader.vs", SOURCE_DIRECTORY + "core/shaders/shader.fs");


	ActorHierarchyLoading();
	ActorPositionCollisionLoading();
	CameraAndControllerLoading();


}

void Scene::UnloadContent()
{
	LOG_INFO("Scene::UnloadContent");

	delete mShader;
	mShader = nullptr;
	delete mCube0;
	mCube0 = nullptr;
	delete mCube1;
	mCube1 = nullptr;
	delete mCube2Player;
	mCube2Player = nullptr;
	delete mPointLight;
	mPointLight = nullptr;
	delete mDirectionalLight;
	mDirectionalLight = nullptr;
	delete mSkybox;
	mSkybox = nullptr;
	delete mStaticMeshActor0;
	mStaticMeshActor0 = nullptr;

	Mesh::ClearCache();
	Material::ClearCache();
	Texture::ClearCache();	
}

void Scene::UpdateSceneGraph(Actor* actor, float dt, Transform globalTransform)
{
	if (!actor) return;

	globalTransform.SetTransformMatrix(globalTransform.GetTransformMatrix() * actor->GetLocalTransformMatrix());

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

	globalTransform.SetTransformMatrix(globalTransform.GetTransformMatrix() * actor->GetLocalTransformMatrix());

	if (auto iRender = dynamic_cast<IRender*>(actor))
	{
		mShader->setMat4("model", globalTransform.GetTransformMatrix());
		iRender->Draw(mShader);
	}

	const auto& children = actor->GetChildren();
	for (Actor* child : children)
	{
		RenderSceneGraph(child, dt, globalTransform);
	}
}

void Scene::UpdatingScene(float dt)
{
	UpdateCurrentController(dt);
	UpdateSceneGraph(&mSceneGraph, dt);
	HandleCollision();
}

void Scene::BindDirectionalLight()
{
	// Bind Directional light
	std::vector<Actor*> directionalLights;
	mSceneGraph.Query<DirectionalLight>(directionalLights);
	if (!directionalLights.empty())
	{
		auto dl = dynamic_cast<DirectionalLight*>(directionalLights[0]);
		mShader->setVec3("dl.direction", glm::normalize(dl->GetDirection()));
		mShader->setVec3("dl.color", dl->mColor);
		mShader->setVec3("dl.ambient", dl->mAmbient);
	}
}

void Scene::BindPointLights()
{
	// Bind Point lights
	std::vector<Actor*> pointLightActors;
	mSceneGraph.Query<PointLight>(pointLightActors);

	mShader->setInt("numPointLights", pointLightActors.size());
	for (int i = 0; i < pointLightActors.size(); i++)
	{
		auto pl = dynamic_cast<PointLight*>(pointLightActors[i]);

		std::string pointLightArrayIndex = "pointLights[" + std::to_string(i) + "]";
		mShader->setVec3(pointLightArrayIndex + ".ambient", pl->mAmbient);
		mShader->setVec3(pointLightArrayIndex + ".color", pl->mColor);
		mShader->setVec3(pointLightArrayIndex + ".position", pl->GetGlobalPosition());
		mShader->setFloat(pointLightArrayIndex + ".constant", pl->mConstant);
		mShader->setFloat(pointLightArrayIndex + ".linear", pl->mLinear);
		mShader->setFloat(pointLightArrayIndex + ".quadratic", pl->mQuadratic);
	}
}

void Scene::BindCamera()
{
	mShader->setMat4("view", mSceneCamera.GetViewMatrix());
	mShader->setMat4("projection", mSceneCamera.GetProjectionMatrix());
	mShader->setVec3("viewPos", mSceneCamera.GetGlobalPosition());
}

void Scene::UpdateCurrentController(float dt)
{
		if (mCurrentController)
		mCurrentController->UpdateCurrentController(dt);
}

void Scene::HandleCollision()
{
	//std::vector<Actor*> actors;
	//mSceneGraph.Query<IBounded>(actors);

	//// Check and resolve collisions
	//for (auto i = 0; i < actors.size(); i++)
	//{
	//	for (auto j = i + 1; j < actors.size(); j++)
	//	{
	//		IBounded* iA = dynamic_cast<IBounded*>(actors[i]);
	//		IBounded* iB = dynamic_cast<IBounded*>(actors[j]);

	//		// Skip intersection if a object ignores collision
	//		if (iA->GetCollisionProperties().IsIgnoreResponse() ||
	//			iB->GetCollisionProperties().IsIgnoreResponse())
	//		{
	//			continue;
	//		}

	//		// Skip intersection checks for two static objects
	//		if (iA->GetCollisionProperties().IsStatic() &&
	//			iB->GetCollisionProperties().IsStatic())
	//		{ 
	//			continue;
	//		}

	//		auto a = iA->GetAABB();
	//		auto b = iB->GetAABB();

	//		glm::vec3 mtv{}; // minimum translation vector to resolve the collision		
	//		if (a.Intersect(b, &mtv)) // This means that the two bounding boxes intersect
	//		{
	//			// Determine how to apply the MTV based on the collision responses of the actors
	//			bool isADynamic = iA->GetCollisionProperties().IsDynamic();
	//			bool isBDynamic = iB->GetCollisionProperties().IsDynamic();

	//			glm::vec3 mtvA(0.f), mtvB(0.f); // Initialize MTV adjustments

	//			if (isADynamic && isBDynamic)
	//			{
	//				// If both actors are dynamic, split the MTV between them
	//				mtvA = -mtv * 0.5f;
	//				mtvB = mtv * 0.5f;
	//			}
	//			else if (isADynamic)
	//			{
	//				// If only actor A is dynamic, apply the full MTV to A
	//				mtvA = -mtv;
	//			}
	//			else if (isBDynamic)
	//			{
	//				// If only actor B is dynamic, apply the full MTV to B
	//				mtvB = mtv;
	//			}
	//			// No adjustment for static objects

	//			// Apply MTV adjustments
	//			if (isADynamic)
	//			{
	//				actors[i]->SetWorldPosition(actors[i]->GetWorldPosition() + mtvA);
	//			}
	//			if (isBDynamic)
	//			{
	//				actors[j]->SetWorldPosition(actors[j]->GetWorldPosition() + mtvB);
	//			}
	//		}
	//	}
	//}
}

void Scene::RenderGUI()
{
	const char* items[] = { "Camera", "Player" };
	static int item_current = 0; 

	ImGui::Begin("Select controller");

	// Combo box
	ImGui::Combo("Select Item", &item_current, items, IM_ARRAYSIZE(items));

	if (item_current == 0)
	{
		mCurrentController = mCameraController;
	}
	else if (item_current == 1)
	{
		mCurrentController = mActorController;
	}

	ImGui::End();
}

void Scene::RenderingScene(float dt)
{
	static bool bShowDemoWindow = false;
	if (bShowDemoWindow)
	{ 
		ImGui::ShowDemoWindow(&bShowDemoWindow);
	}
	glEnable(GL_DEPTH_TEST);

	// Bind Shader, only using 1 shader for now
	mShader->use();

	// Bind
	BindPointLights();
	BindDirectionalLight();
	BindCamera();
	RenderSceneGraph(&mSceneGraph, dt);
	RenderGUI();
	glDepthFunc(GL_LEQUAL);
	mSkybox->RenderSkybox(&mSceneCamera);
}

void Scene::FramebufferSizeCallback(Window* window, int width, int height)
{
	mSceneCamera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

void Scene::MouseMoveCallback(Window* window, double xpos, double ypos)
{
	if (mCurrentController)
	{
		mCurrentController->HandleMouseMovement(window, xpos, ypos);
	}
}

void Scene::MouseButtonCallback(Window* window, int button, int action, int mods)
{
	if (mCurrentController)
	{
		mCurrentController->HandleMouseButton(window, button, action, mods);
	}
}

void Scene::MouseScrollCallback(Window* window, double xoffset, double yoffset)
{
	if (mCurrentController)
	{
		mCurrentController->HandleMouseScroll(window, xoffset, yoffset);
	}
}

void Scene::KeyCallback(Window* window, int key, int scancode, int action, int mods)
{
	if (mCurrentController)
	{
		mCurrentController->HandleKeyboard(window, key, scancode, action, mods);
	}
}

