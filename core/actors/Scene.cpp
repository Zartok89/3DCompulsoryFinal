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

#include <random>

void Scene::GeneratePickups(Material* mat)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-10.f, 10.f);

    for (int p = 0; p < mSpawnAmount; p++)
    {
        glm::vec3 spawnPos { dis(gen), .5f, dis(gen) };
        MeshActor* mPickups{nullptr};
        mPickups = new MeshActor("mPickupObject"+std::to_string(p), Mesh::CreateCube(mat));
        mPickups->SetGlobalPosition(spawnPos);
        mSceneGraph.AddChild(mPickups);
        mPickupVector.emplace_back(mPickups);
    }
}

void Scene::MeshActorLoading(Material* mat)
{
	mSkybox = new Skybox({
		SOURCE_DIRECTORY + "assets/textures/skybox/Daylight Box_Right.png",
		SOURCE_DIRECTORY + "assets/textures/skybox/Daylight Box_Left.png",
		SOURCE_DIRECTORY + "assets/textures/skybox/Daylight Box_Top.png",
		SOURCE_DIRECTORY + "assets/textures/skybox/Daylight Box_Bottom.png",
		SOURCE_DIRECTORY + "assets/textures/skybox/Daylight Box_Front.png",
		SOURCE_DIRECTORY + "assets/textures/skybox/Daylight Box_Back.png",
		});
	mSMAPlayer = new MeshActor("mStaticMeshActor0");
	AssimpLoader::Load(SOURCE_DIRECTORY + "Assets/Models/Horse.fbx", mSMAPlayer);
	mSMABarn = new MeshActor("mSMABarn");
	AssimpLoader::Load(SOURCE_DIRECTORY + "Assets/Models/barn/barn.fbx", mSMABarn);
	mSMABarnDoor = new MeshActor("mSMABarnDoor");
	AssimpLoader::Load(SOURCE_DIRECTORY + "Assets/Models/barn/barnDoor.fbx", mSMABarnDoor);
	mSMAGrassField = new MeshActor("mSMAGrassField");
	AssimpLoader::Load(SOURCE_DIRECTORY + "Assets/Models/barn/GrassField.fbx", mSMAGrassField);

	// Spawning pickups
	mSpawnAmount = 5;
	GeneratePickups(mat);
}

void Scene::LightingActorLoading()
{
	//mPointLight = new PointLight("Point light 0");
	mDirectionalLight = new DirectionalLight("Directional light");
	mDirectionalLightGrass = new DirectionalLight("mDirectionalLightGrass");
}

void Scene::ActorHierarchyLoading()
{
	mSceneGraph.AddChild(&mSceneCamera);
	mSceneGraph.AddChild(mPointLight);
	mSceneGraph.AddChild(mSMAPlayer);
	mSceneGraph.AddChild(mSMABarn);
	mSceneGraph.AddChild(mSMABarnDoor);
	mSceneGraph.AddChild(mSMAGrassField);
	mSMABarn->AddChild(mPointLight);
	mSMABarn->AddChild(mSMABarnDoor);
	mSceneGraph.AddChild(mDirectionalLight);
}

void Scene::ActorPositionCollisionLoading()
{
	mSMAPlayer->SetLocalScale(glm::vec3(0.005f));
	mSMABarn->SetLocalScale(glm::vec3(0.005f));
	mSMAGrassField->SetLocalScale(glm::vec3(0.005f));
	mSMAPlayer->SetLocalRotation(glm::angleAxis((glm::radians(180.f)), glm::vec3(0.f, 1.f, 0.f)));
	mSMABarn->SetLocalRotation(glm::angleAxis((glm::radians(180.f)), glm::vec3(0.f, 1.f, 0.f)));
	mSMAPlayer->SetGlobalPosition({0.f, 0.f, 10.f});
	mSMABarn->SetGlobalPosition({0.f, 0.f, 0.f});
	mSMAPlayer->ChooseCollisionType(2);
	mDirectionalLight->SetLightRotation(-90.f, 1, 0, 0);
	mDirectionalLight->SetLocalPosition(glm::vec3(0.f, 100.f, 0.f));
	mSMABarnDoor->SetLocalPosition(glm::vec3(200.f, 0.f, 0.f));
}

void Scene::CameraAndControllerLoading()
{
	mSceneCamera.SetLocalPosition({ 0.f, 3.f, 20.f });
	mCameraController = std::make_shared<CameraController>(&mSceneCamera);
	mActorController = std::make_shared<ActorController>(mSMAPlayer, false, &mSceneCamera);
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
	delete mPointLight;
	mPointLight = nullptr;
	delete mDirectionalLight;
	mDirectionalLight = nullptr;
	delete mSkybox;
	mSkybox = nullptr;
	delete mSMAPlayer;
	mSMAPlayer = nullptr;
	delete mSMABarn;
	mSMABarn = nullptr;

	for (MeshActor* mesh : mPickupVector)
	{
		delete mesh;
		mesh = nullptr;
	}

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
	const char* items[] = { "FreeCamera", "MovePlayer", "MovePlayerWithCamera" };
	static int item_current = 0; 

	ImGui::Begin("Select controller: ");

	// Combo box
	ImGui::Combo("Select Item", &item_current, items, IM_ARRAYSIZE(items));

	if (item_current == 0)
	{
		mCurrentController = mCameraController;
	}
	else if (item_current == 1)
	{
		mActorController->mIsAttachedToPlayer = false;
		mCurrentController = mActorController;
	}
	else if (item_current == 2)
	{
		mActorController->mIsAttachedToPlayer = true;
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
	BindDirectionalLight();
	BindPointLights();
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

