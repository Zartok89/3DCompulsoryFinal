#pragma once

#include <array>
#include <string>

#include "Texture.h"
#include "glm/vec3.hpp"
#include "shaders/Shader.h"

class Material
{
public:
	/*
	 * Material texture properties - Like material graph in Unreal
	 */
	enum TextureType
	{
		DIFFUSE, // Base color with even light spread
		SPECULAR, // Light spread at an angle
		NORMAL, // Direction and hight information
		OPACITY, // Sets the opacity
		AMOUNTOFTEXTURES // How many textures there are in total
	};

	// Setting default names for the different texture types
	std::array<std::string, static_cast<size_t>(TextureType::AMOUNTOFTEXTURES)> textureDefaultNames =
	{ "material.diffuseMap", "material.specularMap", "material.normalMap", "material.opacityMap"};

    // Material properties which can be affected by the textures
    struct MaterialProperties
    {
        //glm::vec3 mColor {1.f, 1.f, 1.f};
        glm::vec3 mDiffuse{ .5f };
		glm::vec3 mSpecular{ 1.f };
    	glm::vec3 mAmbient{ 0.6f };
        float mShininess{ 32.f };
        float mAlphaCutoff = 1.f;
    };

	/*
	 * Constructor, destructor and copy + mover
	 */
    Material(const std::string& name); // Constructing the material object
	Material(const Material&) = delete; // Delete Copy Constructor
    Material& operator=(const Material&) = delete; // Delete Copy Assignment Operator
	Material(Material&&) = delete;
    Material& operator=(Material&&) = delete;

	/*
	 * Loading and unloading material content
	 */
	static Material* Load(const std::string& name); // Loading a material with no textures
    static Material* Load(const std::string& name, const std::array<Texture*, TextureType::AMOUNTOFTEXTURES>& textures, const MaterialProperties& properties); // Loading a material with textures and properties
    static void Unload(const std::string& name); // Finding the material by name, then deleting it from the cache
    static void ClearCache(); // Clearing the material cache
	void BindMaterialTexture(const Shader* shader) const; // Binding the Material and Textures to the shader

	/*
	 * Getting the texture types and properties
	 */
    Texture* GetTexture(TextureType type) const;
    const MaterialProperties& GetProperties() const { return mProperties; }

	/*
	 * Setting the texture types and properties
	 */
	void SetTexture(TextureType type, Texture* texture);
    void SetProperties(const MaterialProperties& properties) { mProperties = properties; }

	/*
	 * Member variables
	 */
	MaterialProperties mProperties{};
    std::array<Texture*, TextureType::AMOUNTOFTEXTURES> mTextures{};
    static std::unordered_map<std::string, Material*> mCache;


	

	

};
