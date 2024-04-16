#include "Camera.h"

Camera::Camera(
    const std::string& name,
    glm::vec3 position,
    glm::quat rotation,
    float aspectRatio,
    float nearPlane,
    float farPlane,
    float fieldOfView,
    float maxMovementSpeed,
    float accelerationSpeed,
    float dampingFactor)
    : Actor(name)
{
	mMaxMovementSpeed = maxMovementSpeed;
    mAccelerationSpeed = accelerationSpeed;
    mDampingFactor = dampingFactor;
    mAspectRatio = aspectRatio;
    mNearPlane = nearPlane;
    mFarPlane = farPlane;
    mFieldOfView = fieldOfView;
    SetPosition(position);
    SetRotation(rotation);
    UpdateProjectionMatrix();
}

void Camera::Update(float dt)
{
    UpdateVelocity(dt);
    UpdatePosition(dt);
    UpdateDamping(dt);
}

void Camera::SetAspectRatio(float aspectRatio)
{
    mAspectRatio = aspectRatio;
    UpdateProjectionMatrix();
}

glm::vec3 Camera::GetForwardVector() const
{
    return glm::rotate(GetRotation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 Camera::GetUpVector() const
{
    return glm::rotate(GetRotation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(GetPosition(), GetPosition() + GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
    return mProjectionMatrix;
}


glm::mat4 Camera::GetVPMatrix() const
{
    return mProjectionMatrix * GetViewMatrix();
}

void Camera::UpdateVelocity(float dt)
{
    mVelocity += mAcceleration * dt;

    if (glm::length(mVelocity) > mMaxMovementSpeed) 
    {
        mVelocity = glm::normalize(mVelocity) * mMaxMovementSpeed;
    }

    mAcceleration = glm::vec3(0.f);
}

void Camera::UpdateDamping(float dt)
{
    float dampingFactor = glm::length(mVelocity) > 0 ? mDampingFactor : 0.0f;

    mVelocity -= mVelocity * dampingFactor * dt;

    if (glm::length(mVelocity) < 0.001f) 
    {
        mVelocity = glm::vec3(0.0f);
    }
}

void Camera::UpdatePosition(float dt)
{
    glm::vec3 front = GetForwardVector();
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::cross(right, front);

    SetPosition(GetPosition() + mVelocity.x * dt * right + mVelocity.y * dt * up + mVelocity.z * dt * front);
}

void Camera::UpdateProjectionMatrix()
{
    mProjectionMatrix = glm::perspective(glm::radians(mFieldOfView), mAspectRatio, mNearPlane, mFarPlane);
}
