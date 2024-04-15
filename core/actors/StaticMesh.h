#pragma once

class MeshActor : public Actor, public IRender
{
public:
    MeshActor(const std::string& name, Mesh* mesh);
    virtual void Draw(const class Shader* shader = nullptr) const;
    Mesh* mMesh{ nullptr };
};