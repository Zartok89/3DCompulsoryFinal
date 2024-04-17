#include "Material.h"

std::unordered_map<std::string, Material*> Material::mCache;

Material::Material(const std::string& name)
{
    mTextures.fill(nullptr); // Initialize all texture pointers to nullptr
}

Material* Material::Load(const std::string& name)
{
	return Load(name, {}, {});
}

Material* Material::Load(const std::string& name, const std::array<Texture*, TextureType::AMOUNTOFTEXTURES>& textures, const MaterialProperties& properties)
{
    auto it = mCache.find(name);
    if (it != mCache.end())
    {
        return mCache[name];
    }

    Material* material = new Material(name);
    material->mTextures = textures;
    material->mProperties = properties;
    mCache[name] = material;
    return material;
}

void Material::Unload(const std::string& name)
{
    auto it = mCache.find(name);
    if (it != mCache.end())
    {
        delete it->second;
        mCache.erase(it);
    }
}

void Material::ClearCache()
{
	for (auto it : mCache)
    {
        delete it.second;
    }
    mCache.clear();
}

void Material::BindMaterialTexture(const Shader* shader) const
{
    for (size_t i = 0; i < mTextures.size(); ++i)
    {
        if (mTextures[i])
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mTextures[i]->GetTextureID());

            shader->setInt(textureDefaultNames[i], static_cast<int>(i));
        }
    }

    //shader->setVec3("material.color", mProperties.mColor);
    shader->setVec3("material.diffuseColor", mProperties.mColor);
    shader->setFloat("material.shininess", mProperties.mShininess);
}

Texture* Material::GetTexture(TextureType type) const
{
    if (type >= 0 && type < TextureType::AMOUNTOFTEXTURES)
    {
        return mTextures[type];
    }
    return nullptr;
}

void Material::SetTexture(TextureType type, Texture* texture)
{
	if (type >= 0 && type < TextureType::AMOUNTOFTEXTURES)
    {
        mTextures[type] = texture;
    }
}
