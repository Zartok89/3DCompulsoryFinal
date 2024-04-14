#pragma once

#include <string>
#include <vector>

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "utility/Transform.h"

class Actor
{
public:
    enum class TransformSpace 
    {
        Local,
        Global
    };

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

	~Actor(); // Defaulted destructor

    /*
     * Virtual functions
     */
    virtual void Update(float dt) {}; // Update function to be overriden by classes inheriting from Actor

    /*
     * Operations used to add and remove actors to the scene graph
     */
	void SetParent(Actor* parent); // Setting the root of the actor object
    void AddChild(Actor* child); // Adding subobjects to the root
    void RemoveChild(Actor* child); // Removing subobjects to the root

	/*
	 * Setting the actor transforms by using set operations
	 */
    void SetTransform(const Transform& transform) { mTransform = transform; } // Setting the actor transform
    void SetPosition(const glm::vec3& position, Actor::TransformSpace type = Actor::TransformSpace::Local); // Setting the position of the actor in either local or world space
    void SetRotation(const glm::quat& rotation, Actor::TransformSpace type = Actor::TransformSpace::Local); // Setting the rotation of the actor in either local or world space
    void SetScale(const glm::vec3& scale, Actor::TransformSpace type = Actor::TransformSpace::Local); // Setting the scale of the actor in either local or world space

    /*
     * Getting the transform information of the actor
     */
	//Local transforms
	const glm::vec3& GetPosition() const { return mTransform.GetPosition(); } // Getting the local actor position
    const glm::quat& GetRotation() const { return mTransform.GetRotation(); } // Getting the local actor rotation
    const glm::vec3& GetScale() const { return mTransform.GetScale(); } // Getting the local actor scale
    const glm::mat4 GetTransformMatrix() const { return mTransform.GetTransformMatrix(); } // Getting the local actor transform matrix
    const Transform& GetTransform() const { return mTransform; } // Getting the local actor transform
    std::vector<Actor*>& GetChildren() { return mChildren; } // Getting the chrildren of this current actor
    //Global transforms
    glm::vec3 GetGlobalPosition() const; // Getting the local actor position by checking the parents position or this one if its a parent
    glm::quat GetGlobalRotation() const; // Getting the local actor rotation by checking the parents rotation or this one if its a parent
    glm::vec3 GetGlobalScale() const; // Getting the local actor scale by checking the parents scale or this one if its a parent
    glm::mat4 GetGlobalTransformMatrix() const; // Getting the local actor scale by checking the parents scale or this one if its a parent


protected:
    Actor* mParent{nullptr};
    std::vector<Actor*> mChildren;

private:
    TagUnique mTag;
    Transform mTransform{};
};
