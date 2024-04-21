#pragma once
#include <memory>
#include <string>

#include "ActorController.h"
#include "MeshActor.h"
#include "actors/SceneGraph.h"
#include "camera/Camera.h"
#include "camera/CameraController.h"
#include "graphics/Mesh.h"
#include "interfaces/ControllerInterface.h"
#include "lighting/PointLight.h"
#include "lighting/DirectionalLight.h"
#include "Mathematics/ParametricCurve.h"
#include "Mathematics/BarycentricC.h"

class Scene
{

public:
	explicit Scene(const std::string& name); // A constructor which wont allow for implicit conversions
    virtual ~Scene(); // A virtual deconstructor which lets the objects inheriting override the function
	void MaterialTextureLoading(Material*& material);
	void LightingActorLoading();
	void MeshActorLoading(Material* mat);
	void GeneratePickups(Material* mat);
	void PickingUpObjects();
	void OpenDoor(float dt);
	void EnteringHouse();
	void SimpleCollision(MeshActor* player, float playerWidth, float playerLength, 
	MeshActor* otherObject, float otherWidth, float otherLength);
	void NPCWalking(MeshActor* NPC, float dt);
	void ActorHierarchyLoading();
	void ActorPositionCollisionLoading();
	void CameraAndControllerLoading();

	/*
     * Operators
     */
    Scene(const Scene&) = delete; // Delete copy constructor
    Scene& operator=(const Scene&) = delete; // Delete copy assignment operator
    Scene(Scene&&) = delete;  // Delete move constructor
    Scene& operator=(Scene&&) = delete; // Delete move assignment operator

    /*
     * Load/Unload content
     */
    virtual void LoadContent(); // Loading all content related to the spesific overrided scene
    virtual void UnloadContent(); // Unloading all content related to the spesific overrided scene

    /*
     * Updating the Scene and Scene Graph
     */
	void UpdateSceneGraph(Actor* actor, float dt, Transform globalTransform = Transform{});
    void RenderSceneGraph(Actor* actor, float dt, Transform globalTransform = Transform{});
    void UpdatingScene(float dt); // Updating transforms in the scene using deltaTime
    void RenderingScene(float dt); // Rendering the actors in the scene using deltaTime
	void BindDirectionalLight(); // 
    void BindPointLights(); // 
    void BindCamera(); // 
    void UpdateCurrentController(float dt); // Updating the current controller
    void HandleCollision(); // Handling the collision updating objects
    void RenderGUI(); // Render the ingame GUI

    /*
     * Callbacks used in the spesific scenes
     */
    void FramebufferSizeCallback(class Window* window, int width, int height); // Window callbacks related to the spesific scene
    void MouseMoveCallback(class Window* window, double xpos, double ypos); // Mouse movement callbacks related to the spesific scene
    void MouseButtonCallback(class Window* window, int button, int action, int mods); // Mouse button callbacks related to the spesific scene
    void MouseScrollCallback(class Window* window, double xoffset, double yoffset); // Mouse scroll callbacks related to the spesific scene
    void KeyCallback(class Window* window, int key, int scancode, int action, int mods); // Keyboard key callbacks related to the spesific scene

    /*
     * Scene Controllers
     */
	std::shared_ptr<IController> GetController() const { return mCurrentController; }
    void SetController(const std::shared_ptr<IController>& controller) { mCurrentController = controller; }
	std::shared_ptr<ActorController> mActorController;
    std::shared_ptr<CameraController> mCameraController;
	std::shared_ptr<IController> mCurrentController{ nullptr };

    /*
     * Member Variables
     */
	SceneGraph mSceneGraph;
    Camera mSceneCamera{ "SceneCamera" };

    int mSpawnAmount;
    std::vector<MeshActor*> mPickupVector;

    class Shader* mShader{ nullptr };
    class Skybox* mSkybox{ nullptr };

	PointLight* mPointLight{ nullptr };
	DirectionalLight* mDirectionalLight{ nullptr };
	DirectionalLight* mDirectionalLightGrass{ nullptr };
	ParametricCurve* ParaCurve;
    MeshActor* mSMAInterpolation{ nullptr };
    MeshActor* mSMAPlane{ nullptr };

	std::unordered_map<std::string, MeshActor*> ActorMap;
    bool bDoorIsClosed = true;
	int c = 0; // count variable
    	float t = 0.0f; // Parameter for interpolation
	int currentIndex = 0; // Current index in the points vector
	int direction = 1; // Direction of movement, 1 for forward, -1 for backward

};
