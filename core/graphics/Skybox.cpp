#include "Skybox.h"

#include "Mesh.h"
#include "stb_image.h"
#include "camera/Camera.h"
#include "glad/glad.h"
#include "glm/fwd.hpp"
#include "shaders/Shader.h"
#include "utility/Defines.h"
#include "utility/Logger.h"

Skybox::Skybox(std::initializer_list<std::string> texturePaths)
{
    mSkyboxShader = new Shader(SOURCE_DIRECTORY + "core/shaders/skybox.vs", SOURCE_DIRECTORY + "core/shaders/skybox.fs");
    mMesh = Mesh::CreateCube(nullptr);

    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);

    stbi_set_flip_vertically_on_load(false);

    int width, height, nrChannels, index{0};
    for (auto texturePath : texturePaths)
    {
        unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index++,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );  
        }
        else
        {
            LOG_ERROR("Cubemap texture failed to load at location: %s", texturePath.c_str());
        }     
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Skybox::~Skybox()
{
    // Mesh deletes itself
    glDeleteTextures(1, &mTextureId);
    delete mSkyboxShader;
}

void Skybox::RenderSkybox(Camera* renderCamera)
{
    if (!renderCamera)
        return;

    mSkyboxShader->use();
    // Set to mat3 to remove the translation
    mSkyboxShader->setMat4("view", glm::mat3(renderCamera->GetViewMatrix()));
    mSkyboxShader->setMat4("projection", renderCamera->GetProjectionMatrix());

    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureId);
    mMesh->DrawMesh(mSkyboxShader);    
}