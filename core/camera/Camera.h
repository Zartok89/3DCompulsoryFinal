#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "actors/Actor.h"

class Camera : public Actor
{

public:
    //Camera Constructor 
    Camera(const std::string& name,
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::quat rotation = glm::quat(glm::identity<glm::quat>()),
        float aspectRatio = 1280.f / 720.f,
        float nearPlane = 0.1f,
        float farPlane = 500.0f,
        float fieldOfView = 45.0f,
        float maxMovementSeed = 40.f,
        float accelerationSpeed = 20.0f,
        float dampingFactor = 5.0f);

    virtual void Update(float dt) override; // Updating  Camera Veclocity, Position and Dampening

    /*
     * Adding parameter values to camera
     */
    void AddVelocity(const glm::vec3& velocity) { mVelocity += velocity; }

    /*
     * Setting camera parameters
     */
    void SetAspectRatio(float aspectRatio); // Setting the aspectratio of the camera
    void SetVelocity(const glm::vec3& velocity) { mVelocity = velocity; } // Setting the velocity of the camera
    void SetAccelerationSpeed(float accelerationSpeed) { mAccelerationSpeed = accelerationSpeed; } // Setting the acceleration speed of the camera
    void SetAcceleration(const glm::vec3& acceleration) { mAcceleration = acceleration; } // Setting the acceleration of the camera
    void SetMaxMovementSpeed(float movementSpeed) { mMaxMovementSpeed = movementSpeed; } // Setting the maximum movement speed of the camera

    /*
     * Getting camera parameters
     */
    float GetMaxMovementSpeed() const { return mMaxMovementSpeed; } // Getting the maximum movement speed of the camera
    float GetAspectRatio() const { return mAspectRatio; } // Getting the aspectratio of the camera
    float GetNearPlane() const { return mNearPlane; } // Getting the near plane of the camera, at which length it start clipping
    float GetFarPlane() const { return mFarPlane; } // Getting the near plane of the camera, at which length it stops rendering
    float GetFieldOfView() const { return mFieldOfView; } // Getting the field of view of the camera
    float GetAccelerationSpeed() const{ return mAccelerationSpeed; } // Getting the acceleration speed of the camera
    const glm::vec3& GetAcceleration() const{ return mAcceleration; } // Getting the acceleration of the camera
    const glm::vec3& GetVelocity() const { return mVelocity; } // Getting the velocity of the camera
    glm::vec3 GetForwardVector() const; // Getting the forward vector of the camera
    glm::vec3 GetUpVector() const; // Getting the up vector of the camera
    glm::mat4 GetViewMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;
    glm::mat4 GetVPMatrix() const;

private:
    void UpdateVelocity(float dt);    
    void UpdatePosition(float dt);
    void UpdateDamping(float dt);
    void UpdateProjectionMatrix();

    float mMaxMovementSpeed;
    float mAccelerationSpeed;
    float mDampingFactor;
    glm::vec3 mVelocity{0.f, 0.f, 0.f};
    glm::vec3 mAcceleration{0.f, 0.f, 0.f};

    float mAspectRatio;
    float mNearPlane;
    float mFarPlane;
    float mFieldOfView;

    glm::mat4 mProjectionMatrix;
};

