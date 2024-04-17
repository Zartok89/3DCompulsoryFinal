#pragma once
#include "Material.h"
#include "utility/Types.h"
#include "Vertex.h"

class Mesh
{
public:
    /*
     * Constructor and deconstructor
     */
    explicit Mesh(const std::string& name, std::vector<Vertex>&& vertices, std::vector<Index>&& indices, Material* material); // A constructor which wont allow for implicit conversions
    virtual ~Mesh(); // A virtual deconstructor which lets the objects inheriting override the function

    /*
     * Drawing, loading, unloading and clearing the mesh cache 
     */
	void SetupMesh(); // Setting up, generating and binds vertex and buffer
	void DrawMesh(const Shader* shader) const; // Binding the VAO and drawing the mesh
    static Mesh* Load(const std::string& path); // Loading all the meshes in a given path from the cache
    static void Unload(const std::string& path); // Erasing all the meshes in a given path from the cache
    static void ClearCache(); // Clearing the cache

    /*
     * Getting and setting mesh material
     */
	void SetMeshMaterial(Material* material) { mMaterial = material; }
    const Material* GetMeshMaterial() const { return mMaterial; }

    /*
     * Test functions
     */
    static Mesh* CreateCube(Material* material);

    /*  
     * Member Variables
     */
    Mesh* mMesh{ nullptr };
	std::vector<Vertex> mVertices{};
    std::vector<Index> mIndices{};
    Material* mMaterial{nullptr};

    VAO mVAO{ 0U };
    VBO mVBO{ 0U };
    EBO mEBO{ 0U };

    static std::unordered_map<std::string, Mesh*> mCache;
};
