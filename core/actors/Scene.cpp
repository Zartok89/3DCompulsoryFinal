#include "Scene.h"

#include <imgui.h>
#include <memory>

#include "camera/CameraController.h"
#include "shaders/Shader.h"
#include "utility/Defines.h"
#include "utility/Logger.h"
#include "Interfaces/RenderInterface.h"
#include "actors/Actor.h"
#include "GLFW/glfw3.h"
#include "graphics/Skybox.h"
#include "utility/AssimpLoader.h"
#include "Mathematics/ParametricCurve.h"
#include <random>
#include <string>

Scene::Scene(const std::string& name) : mSceneGraph(name) {}

Scene::~Scene() {}

void Scene::GeneratePickups(Material* mat)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-10.f, 10.f);

	for (int p = 0; p < mSpawnAmount; p++)
	{
		glm::vec3 spawnPos{ dis(gen), .5f, dis(gen) };
		MeshActor* mPickups{ nullptr };
		mPickups = new MeshActor("mPickupObject" + std::to_string(p), Mesh::CreateCube(mat));
		mPickups->SetGlobalPosition(spawnPos);
		mPickups->SetLocalScale(glm::vec3(0.5f, 3.f, 0.5f));
		mSceneGraph.AddChild(mPickups);
		mPickupVector.emplace_back(mPickups);
	}
}

void Scene::PickingUpObjects()
{
	float pickupRange = 0.5f;
	for (const auto pickups : mPickupVector)
	{
		auto pickupPos = pickups->GetGlobalPosition();
		auto playerPos = ActorMap["Player"]->GetGlobalPosition();
		if ((playerPos.x <= pickupPos.x + pickupRange && playerPos.x >= pickupPos.x - pickupRange) &&
			(playerPos.z <= pickupPos.z + pickupRange && playerPos.z >= pickupPos.z - pickupRange))
		{
			mSceneGraph.RemoveChild(pickups);
		}
	}
}

void Scene::OpenDoor(float dt)
{
	float collisionRangeX = 6.f;
	float collisionRangeZ = 3.f;
	glm::vec3 barnLocation = ActorMap["Barn"]->GetGlobalPosition();
	auto playerPos = ActorMap["Player"]->GetGlobalPosition();

	if ((playerPos.x <= barnLocation.x && playerPos.x >= barnLocation.x - 4) &&
		(playerPos.z <= barnLocation.z + collisionRangeZ && playerPos.z >= barnLocation.z - collisionRangeZ))
	{
		if (bDoorIsClosed)
		{
			float lerp = ActorMap["BarnDoor"]->GetLocalPosition().x;
			float maxRange = -700.f;
			lerp -= dt * 1000;
			if (lerp >= maxRange)
			{
				ActorMap["BarnDoor"]->SetLocalPosition(glm::vec3(lerp, 0.f, 0.f));
			}
			else
			{
				bDoorIsClosed = false;
			}
		}
	}
	else
	{
		float lerp = ActorMap["BarnDoor"]->GetLocalPosition().x;
		float originalxRange = 0.f;
		lerp += dt * 1000;
		if (lerp <= originalxRange)
		{
			ActorMap["BarnDoor"]->SetLocalPosition(glm::vec3(lerp, 0.f, 0.f));
		}
		else
		{
			bDoorIsClosed = true;
		}
	}
}

void Scene::EnteringHouse()
{
	float collisionRangeX = 5.f;
	float collisionRangeZ = 3.f;
	glm::vec3 barnLocation = ActorMap["Barn"]->GetGlobalPosition();
	auto playerPos = ActorMap["Player"]->GetGlobalPosition();

	if ((playerPos.x <= barnLocation.x + collisionRangeX && playerPos.x >= barnLocation.x - collisionRangeX) &&
		(playerPos.z <= barnLocation.z + collisionRangeZ && playerPos.z >= barnLocation.z - collisionRangeZ))
	{
		mActorController->mIsAttachedToPlayer = false;
		mSceneCamera.SetGlobalPosition(glm::vec3{ 4.5f, 1.f, 0.f });
		mSceneCamera.SetGlobalRotation(glm::angleAxis((glm::radians(90.f)), glm::vec3(0.f, 1.f, 0.f)));
	}
}

void Scene::SimpleCollision(MeshActor* player, float playerWidth, float playerLength,
	MeshActor* otherObject, float otherWidth, float otherLength)
{
	glm::vec3 playerPos = player->GetGlobalPosition();
	glm::vec3 otherPos = otherObject->GetGlobalPosition();

	if ((playerPos.x - playerWidth <= otherPos.x + otherWidth) && (playerPos.x + playerWidth >= otherPos.x - otherWidth))
	{
		std::cout << "Collisjon\n";
	}
}

glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t)
{
	return a + t * (b - a);
}

void Scene::NPCWalking(MeshActor* NPC, float dt)
{
	if (bNPCFollowCurve)
	{
		std::vector<Vertex> tempVec = mSMAInterpolation->mMesh->mVertices;

		t += dt * direction;

		if (t >= -10.0f) {
			t = 0.0f;
			currentIndex += direction;
			if (currentIndex == tempVec.size() - 1 || currentIndex == 0) {
				direction *= -1;
			}
		}
		glm::vec3 curveInfo = mSMAInterpolation->GetGlobalPosition();
		glm::vec3 currentPosition = lerp(tempVec[currentIndex].mPosition + curveInfo, tempVec[currentIndex + direction].mPosition + curveInfo, t);
		NPC->SetGlobalPosition(currentPosition);
	}
}

//void Scene::SimpleCollision(MeshActor& Player, MeshActor& otherObjec)
//{
//	float collisionRangeX = 5.f;
//	float collisionRangeZ = 3.5f;
//	float wallThickness = 1.f;
//	glm::vec3 barnLocation = ActorMap["Barn"]->GetGlobalPosition();
//	auto playerPos = ActorMap["Player"]->GetGlobalPosition();
//	if ((playerPos.x <= barnLocation.x + collisionRangeX && playerPos.x >= barnLocation.x + collisionRangeX - wallThickness) &&
//	(playerPos.z <= barnLocation.z + collisionRangeZ && playerPos.z >= barnLocation.z - collisionRangeZ ))
//	{
//		std::cout << "house detected\n";
//	}
//	if ((playerPos.x <= barnLocation.x - collisionRangeX + wallThickness && playerPos.x >= barnLocation.x - collisionRangeX ) &&
//	(playerPos.z <= barnLocation.z + collisionRangeZ && playerPos.z >= barnLocation.z - collisionRangeZ ))
//	{
//		std::cout << "house detected\n";
//		//ActorMap["Player"]->SetGlobalPosition(glm::vec3{ActorMap["Player"]->GetGlobalPosition().x - .2f, ActorMap["Player"]->GetGlobalPosition().y, ActorMap["Player"]->GetGlobalPosition().z});
//	}
//}

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

	ActorMap["Player"] = new MeshActor("mSMAPlayer");
	AssimpLoader::Load(SOURCE_DIRECTORY + "Assets/Models/Horse.fbx", ActorMap["Player"]);
	ActorMap["Barn"] = new MeshActor("mSMABarn");
	AssimpLoader::Load(SOURCE_DIRECTORY + "Assets/Models/barn/barn.fbx", ActorMap["Barn"]);
	ActorMap["BarnDoor"] = new MeshActor("mSMABarnDoor");
	AssimpLoader::Load(SOURCE_DIRECTORY + "Assets/Models/barn/barnDoor.fbx", ActorMap["BarnDoor"]);
	ActorMap["GrassField"] = new MeshActor("mSMAGrassField");
	AssimpLoader::Load(SOURCE_DIRECTORY + "Assets/Models/barn/GrassField.fbx", ActorMap["GrassField"]);
	ActorMap["NPC"] = new MeshActor("mSMANPC");
	AssimpLoader::Load(SOURCE_DIRECTORY + "Assets/Models/HorseNPC.fbx", ActorMap["NPC"]);
	ActorMap["BarnHay"] = new MeshActor("mSMABarnHay");
	AssimpLoader::Load(SOURCE_DIRECTORY + "Assets/Models/barn/barnHay.fbx", ActorMap["BarnHay"]);
	// Spawning pickups
	mSpawnAmount = 5;
	GeneratePickups(mat);

	// Interpolation Curve
	ParaCurve = new ParametricCurve();
	mSMAInterpolation = new MeshActor("InterpolationCurve");
	mSMAInterpolation->mMesh = ParaCurve->CreateInterpolationCurve3Points(0, 13, 0.2f);
	mSMAInterpolation->mMesh->DrawLine = true;

	// Plane
	mSMAPlane = new MeshActor("Plane");
	mSMAPlane->mMesh = BarycentricC::CreatePlane(-5,-10,5,20,0.1f);


}

void Scene::LightingActorLoading()
{
	mPointLight = new PointLight("Point light 0");
	mDirectionalLight = new DirectionalLight("Directional light");
	mDirectionalLightGrass = new DirectionalLight("mDirectionalLightGrass");
}

void Scene::ActorHierarchyLoading()
{
	mSceneGraph.AddChild(&mSceneCamera);
	mSceneGraph.AddChild(ActorMap["NPC"]);
	mSceneGraph.AddChild(ActorMap["Player"]);
	mSceneGraph.AddChild(ActorMap["Barn"]);
	mSceneGraph.AddChild(ActorMap["GrassField"]);
	ActorMap["Barn"]->AddChild(mPointLight);
	ActorMap["Barn"]->AddChild(ActorMap["BarnDoor"]);
	ActorMap["Barn"]->AddChild(ActorMap["BarnHay"]);
	mSceneGraph.AddChild(mDirectionalLight);
	mSceneGraph.AddChild(mSMAInterpolation);
	//mSceneGraph.AddChild(mSMAPlane);
}

void Scene::ActorPositionCollisionLoading()
{
	ActorMap["Player"]->SetLocalScale(glm::vec3(0.005f));
	ActorMap["NPC"]->SetLocalScale(glm::vec3(0.005f));
	ActorMap["Barn"]->SetLocalScale(glm::vec3(0.005f));
	ActorMap["GrassField"]->SetLocalScale(glm::vec3(0.005f));
	ActorMap["Player"]->SetLocalRotation(glm::angleAxis((glm::radians(180.f)), glm::vec3(0.f, 1.f, 0.f)));
	ActorMap["Barn"]->SetLocalRotation(glm::angleAxis((glm::radians(180.f)), glm::vec3(0.f, 1.f, 0.f)));
	ActorMap["Player"]->SetGlobalPosition({ 0.f, 0.f, 10.f });
	ActorMap["NPC"]->SetGlobalPosition({ 0.f, 0.f, 0.f });
	ActorMap["Barn"]->SetGlobalPosition({ 0.f, 0.f, 0.f });
	mSMAInterpolation->SetGlobalPosition({ 10.f, 0, 0.f });
	//ActorMap["Player"]->ChooseCollisionType(2);
	//mDirectionalLight->SetLightRotation(90.f, 1, 0, 0);
	mDirectionalLight->SetLightRotation(normalize(glm::vec3(-0.7f, -1.0f, -0.3f)));
	mDirectionalLight->SetLocalPosition(glm::vec3(0.f, 100.f, 0.f));
	mSMAPlane->SetGlobalPosition({-40.f, -2.f, 0.f});


	//Collision handling
	//std::string PlayerModel = "Assets/Models/Horse.fbx";
	//BarycentricC::getBarycentricCoordinatesActor(PlayerModel, ActorMap["Player"]->mMesh); 
}

void Scene::CameraAndControllerLoading()
{
	mSceneCamera.SetLocalPosition({ 0.f, 3.f, 20.f });
	mCameraController = std::make_shared<CameraController>(&mSceneCamera);
	mActorController = std::make_shared<ActorController>(ActorMap["Player"], true, &mSceneCamera);

	mCurrentController = mActorController;
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
	delete ActorMap["Player"];
	ActorMap["Player"] = nullptr;
	delete ActorMap["Barn"];
	ActorMap["Barn"] = nullptr;
	ActorMap["BarnDoor"] = nullptr;
	delete ActorMap["BarnDoor"];
	ActorMap["GrassField"] = nullptr;
	delete ActorMap["GrassField"];

	delete mSMAInterpolation;
	mSMAInterpolation = nullptr; 
	delete mSMAPlane;
	mSMAPlane = nullptr; 

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
	NPCWalking(ActorMap["NPC"], dt);
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
	const char* items[] = { "MovePlayerWithCamera", "MovePlayer", "FreeCamera" };
	static int item_current = 0;

	ImGui::Begin("Select controller: ");

	// Combo box
	ImGui::Combo("Camera Type", &item_current, items, IM_ARRAYSIZE(items));

	if (item_current == 0)
	{
		mActorController->mIsAttachedToPlayer = true;
		mCurrentController = mActorController;
	}
	else if (item_current == 1)
	{
		mActorController->mIsAttachedToPlayer = false;
		mCurrentController = mActorController;
	}
	else if (item_current == 2)
	{
		mCurrentController = mCameraController;
	}

	ImGui::Text("Player x value = %f", ActorMap["Player"]->GetGlobalPosition().x);
	ImGui::Text("Player z value = %f", ActorMap["Player"]->GetGlobalPosition().z);

	if (ImGui::Button(bNPCFollowCurve ? "NPC ENABLED" : "NPC DISABLED"))
	{
        bNPCFollowCurve = !bNPCFollowCurve;
	}


    if (ImGui::Checkbox("Enable CurvedGround", &bDrawBarycentricPlane)) 
	{

	    if (bDrawBarycentricPlane) 
		{
			mSceneGraph.AddChild(mSMAPlane);
	    }
	    else 
		{
			mSceneGraph.RemoveChild(mSMAPlane);
	    }
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

	// Draw stuff
	RenderSceneGraph(&mSceneGraph, dt);
	RenderGUI();
	glDepthFunc(GL_LEQUAL);
	mSkybox->RenderSkybox(&mSceneCamera);
	PickingUpObjects();
	OpenDoor(dt);
	EnteringHouse();
	//NPCWalking(ActorMap["NPC"], dt);
	//SimpleCollision(ActorMap["Player"], .5f, .5f,
	//	ActorMap["BarnHay"], 2.f, 1.f);
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