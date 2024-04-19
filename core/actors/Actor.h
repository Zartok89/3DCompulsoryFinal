#pragma once

#include <vector>

#include "utility/Transform.h"
#include "utility/Tag.h"

class Actor
{
public:

    /*
     * Constructur, Deconstructor and copy + movers
     */
    Actor(const std::string& name); // Defaulted constructor

	// Deleted copy constructor and copy assignment operator
	Actor(const Actor&) = delete;
    Actor& operator=(const Actor&) = delete;

    // Defaulted move constructor and move assignment operator
	Actor(Actor&&) = default;
    Actor& operator=(Actor&&) = default;

    virtual ~Actor(); // Defaulted destructor

    /*
     * Virtual functions
     */
    virtual void Update(float dt) {} // Update function to be overriden by classes inheriting from Actor

    /*
     * Operations used to add and remove actors to the scene graph
     */
	void SetParent(Actor* parent); // Setting the root of the actor object
    void AddChild(Actor* child); // Adding subobjects to the root
    void RemoveChild(Actor* child); // Removing subobjects to the root

	/*
	 * Setting the actor transforms by using set operations
	 */
    //Local transforms
    void SetTransform(const Transform& transform) { mTransform = transform; } // Setting the actor transform
    void SetLocalTransformMatrix(const glm::mat4& transformMatrix) { mTransform.SetTransformMatrix(transformMatrix); }
	void SetLocalPosition(const glm::vec3& position) { mTransform.SetPosition(position); } // Setting the position of the actor in either local or world space
    void SetLocalRotation(const glm::quat& rotation) { mTransform.SetRotation(rotation); } // Setting the rotation of the actor in either local or world space
    void SetLocalScale(const glm::vec3& scale) { mTransform.SetScale(scale); } // Setting the scale of the actor in either local or world space
    //Global transforms
	void SetGlobalPosition(const glm::vec3& position); // Setting the position of the actor in either local or world space
    void SetGlobalRotation(const glm::quat& rotation); // Setting the rotation of the actor in either local or world space
    void SetGlobalScale(const glm::vec3& scale); // Setting the scale of the actor in either local or world space


    /*
     * Getting the transform information of the actor
     */
	//Local transforms
	const glm::vec3& GetLocalPosition() const { return mTransform.GetPosition(); } // Getting the local actor position
    const glm::quat& GetLocalRotation() const { return mTransform.GetRotation(); } // Getting the local actor rotation
    const glm::vec3& GetLocalScale() const { return mTransform.GetScale(); } // Getting the local actor scale
    const glm::mat4 GetLocalTransformMatrix() const { return mTransform.GetTransformMatrix(); } // Getting the local actor transform matrix
    const Transform& GetTransform() const { return mTransform; } // Getting the local actor transform
    glm::vec3 GetRightVector() const { return mTransform.GetRightVector(); }
    std::vector<Actor*>& GetChildren() { return mChildren; } // Getting the chrildren of this current actor
    //Global transforms
    glm::vec3 GetGlobalPosition() const; // Getting the local actor position by checking the parents position or this one if its a parent
    glm::quat GetGlobalRotation() const; // Getting the local actor rotation by checking the parents rotation or this one if its a parent
    glm::vec3 GetGlobalScale() const; // Getting the local actor scale by checking the parents scale or this one if its a parent
    glm::mat4 GetGlobalTransformMatrix() const; // Getting the local actor scale by checking the parents scale or this one if its a parent

    // Query template function
	template <typename T>
    void Query(std::vector<Actor*>& actors)
    {
        if (dynamic_cast<T*>(this))
            actors.emplace_back(this);

        for (auto child : this->mChildren)
            child->Query<T>(actors);
    }

    /*
     * Member variables
     */
    Actor* mParent{nullptr};
    std::vector<Actor*> mChildren;
    TagUnique mTag;
    Transform mTransform{};
};