#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include "utility/Types.h"

class Texture
{
    TextureID mId{ 0U }; // Texture ID
    std::string mPath; // The texture path

public:
	Texture(const std::string& path); // Texture constructor
    ~Texture(); // Deconstructor of the texture

	// Deleted copy constructor and copy assignment operator
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Deleted move constructor and move assignment operator
    Texture(Texture&&) = delete;
    Texture& operator=(Texture&&) = delete;

    static Texture* Load(const std::string& path); // Loading the texture into the cache
    static void Unload(const std::string& path); // Deleting the cache
    static void ClearCache() { mCache.clear(); } // Clearing the content of the mCache 
    
    TextureID GetTextureID() const { return mId; } // Recieving the texture ID
    bool IsValid() const { return mId == 0; } // Checking if the texture is valid

private:
    void LoadTexture(); // Loading the texture image and binding the texture

    static std::unordered_map<std::string, Texture*> mCache; // Prefer to use unordered map if the index position and ordering is not important
};