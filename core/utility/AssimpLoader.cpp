#include "AssimpLoader.h"
#include "utility/AssimpUtility.h"
#include "utility/Shared.h"
#include "utility/Logger.h"
#include "graphics/Mesh.h"
#include <exception>

#include "actors/MeshActor.h"
#include "lighting/PointLight.h"


std::string AssimpLoader::msBasePath = "";
long long AssimpLoader::msNameIndex = 0;
long long AssimpLoader::msFlags = 0;

void AssimpLoader::Load(const std::string& path, Actor* staticMeshActor, unsigned flags)
{
    Assimp::Importer importer;
    //unsigned int processFlags =
    // aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
    // aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
    // aiProcess_ValidateDataStructure | // perform a full validation of the loader's output
    // aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
    // //aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space (by default right-handed, for OpenGL)
    // aiProcess_SortByPType | // ?
    // aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
    // aiProcess_RemoveRedundantMaterials | // remove redundant materials
    // aiProcess_FindDegenerates | // remove degenerated polygons from the import
    // aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
    // aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
    // aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
    // aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
    // aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
    // //aiProcess_OptimizeMeshes | // join small meshes, if possible;
    // aiProcess_PreTransformVertices | //-- fixes the transformation issue.
    // aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
    // aiProcess_FlipUVs |
	// aiProcess_FlipWindingOrder | // Flip winding order, midlertidig l�sning.
	// 0;

    const aiScene* scene = importer.ReadFile(path, flags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG_INFO("AssimpLoader::Load::Failed: %s", importer.GetErrorString());
        throw std::runtime_error("AssimpLoader::Load::Failed");
    }

    msBasePath = path;
    msFlags = flags;
    ProcessLights(scene, staticMeshActor);
    ProcessNode(scene, scene->mRootNode, staticMeshActor);
}

void AssimpLoader::ProcessLights(const aiScene* scene, Actor* actor)
{
    LOG_INFO("AssimpLoader::ProcessLights::NumLights: %i", scene->mNumLights);
    for (size_t i = 0; i < scene->mNumLights; i++)
    {
        // Skip lights that aren't pointlights
        if (scene->mLights[i]->mType != aiLightSource_POINT)
        {
            continue;
        }

        PointLight* pointLightActor = new PointLight(scene->mLights[i]->mName.C_Str());

        // Transform the pointlight to its world position
        aiNode* lightNode = scene->mRootNode->FindNode(scene->mLights[i]->mName);
        aiMatrix4x4 lightTransform = GetTransformationMatrix(lightNode);
        aiVector3D worldPosition = TransformPosition(lightTransform, scene->mLights[i]->mPosition);

        glm::vec3 pos =
        {
            worldPosition.x,
            worldPosition.y,
            worldPosition.z
        };
        pointLightActor->mAmbient =
        {
            scene->mLights[i]->mColorAmbient.r,
            scene->mLights[i]->mColorAmbient.g,
            scene->mLights[i]->mColorAmbient.b
        };
        pointLightActor->mColor =
        {
            scene->mLights[i]->mColorDiffuse.r,
            scene->mLights[i]->mColorDiffuse.g,
            scene->mLights[i]->mColorDiffuse.b
        };

        pointLightActor->mColor = glm::clamp(pointLightActor->mColor, 0.f, 1.f);
        pointLightActor->SetGlobalPosition(pos);
        actor->AddChild(pointLightActor);
    }
}

void AssimpLoader::ProcessNode(const aiScene* scene, aiNode* node, Actor* parentActor)
{
    LOG_INFO("AssimpLoader::ProcessNode::NodeName: %s", node->mName.C_Str());

    Actor* actor = nullptr;
    for (auto i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::string collisionPrefix, lightPrefix;

        // Make a unique name for the mesh
        std::string modelName = RemoveFileExtension(GetFileNameFromPath(msBasePath));
        std::string meshName = mesh->mName.C_Str();
        std::string actorName = modelName + "_" + meshName + std::to_string(msNameIndex++);





/*        if (HasCollisionPrefix(mesh->mName.C_Str(), collisionPrefix))
        {
            LOG_INFO("AssimpLoader::ProcessNode::CollisionPrefix: %s", collisionPrefix.c_str());
            AABB aabb = ProcessCollisionAABB(mesh);
            actor = new AABBActor(actorName, aabb);
        }
        else */if (HasLightPrefix(mesh->mName.C_Str(), lightPrefix))
        {
            LOG_INFO("AssimpLoader::ProcessNode::LightPrefix: %s", lightPrefix.c_str());
        }
        else
        {
            LOG_INFO("AssimpLoader::ProcessMesh::Name: %s", mesh->mName.C_Str());
            Mesh* internalMesh = ProcessMesh(mesh);

            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            LOG_INFO("AssimpLoader::ProcessMaterial::Name: %s", material->GetName().C_Str());
            if (material)
            {
                Material* internalMaterial = ProcessMaterial(material);
                internalMesh->SetMeshMaterial(internalMaterial);
            }

            auto* meshActor = new MeshActor(actorName, internalMesh);
            // Set the collision properties for the actor to ignore. This should be set to BLOCK when culling the scene in a different collision channel.
            //meshActor->mCollisionProperties.mResponse = CollisionResponse::IGNORE;
            actor = meshActor;
        }
    }

    if (!actor)
    {
        // This means there is no mesh associated with the node. So a new node actor is created with a unique name.
        actor = new Actor(std::string(node->mName.C_Str()) + "_index" + std::to_string(msNameIndex++));
    }

    // Apply node transform
    actor->SetLocalTransformMatrix(AiMatrix4x4ToGlm(node->mTransformation));

    // Add node as a child
    parentActor->AddChild(actor);

    for (auto i = 0; i < node->mNumChildren; ++i)
    {
        ProcessNode(scene, node->mChildren[i], actor);
    }

    LOG_INFO("----------------------------------------------------\n");
}

Mesh* AssimpLoader::ProcessMesh(aiMesh* mesh)
{
    std::vector<Vertex> vertices;
    std::vector<Index> indices;

    for (auto i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 pos{ 0.f };
        pos.x = mesh->mVertices[i].x;
        pos.y = mesh->mVertices[i].y;
        pos.z = mesh->mVertices[i].z;

        glm::vec3 normal{ 0.f, 1.f, 0.f };
        if (mesh->HasNormals())
        {
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
        }

        glm::vec2 texCoords{0.f};
        if (mesh->HasTextureCoords(0))
        {
            texCoords.x = mesh->mTextureCoords[0][i].x;
            texCoords.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back({ pos, normal, texCoords });
    }

    for (auto i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (auto j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    return new Mesh(std::string(mesh->mName.C_Str()), std::move(vertices), std::move(indices), nullptr);
}

Material* AssimpLoader::ProcessMaterial(aiMaterial* material)
{
    // Creates a new material and a default white and black texture set to diffuse and specular.
    Material* internalMaterial = Material::Load(std::string(material->GetName().C_Str()), {}, {});
    internalMaterial->SetTexture(Material::DIFFUSE, Texture::LoadWhiteTexture());
    internalMaterial->SetTexture(Material::SPECULAR, Texture::LoadBlackTexture());

    // Diffuse
    if (0 < material->GetTextureCount(aiTextureType_DIFFUSE))
    {
        aiString str;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
        std::string texturePath = GetDirectoryPath(msBasePath) + "textures/" + std::string(str.C_Str());
        internalMaterial->SetTexture(Material::DIFFUSE, Texture::Load(texturePath));
        LOG_INFO("AssimpLoader::ProcessMaterial::Texture::Diffuse::Path: %s", texturePath.c_str());
    }
    // Specular
    if (0 < material->GetTextureCount(aiTextureType_SPECULAR))
    {
        aiString str;
        material->GetTexture(aiTextureType_SPECULAR, 0, &str);
        std::string texturePath = GetDirectoryPath(msBasePath) + "textures/" + std::string(str.C_Str());
        internalMaterial->SetTexture(Material::SPECULAR, Texture::Load(texturePath));
        LOG_INFO("AssimpLoader::ProcessMaterial::Texture::Specular::Path: %s", texturePath.c_str());
    }
    // Normal
    if (0 < material->GetTextureCount(aiTextureType_NORMALS))
    {
        aiString str;
        material->GetTexture(aiTextureType_NORMALS, 0, &str);
        std::string texturePath = GetDirectoryPath(msBasePath) + "textures/" + std::string(str.C_Str());
        internalMaterial->SetTexture(Material::NORMAL, Texture::Load(texturePath));
        LOG_INFO("AssimpLoader::ProcessMaterial::Texture::Normal::Path: %s", texturePath.c_str());
    }
    // Opacity
    if (0 < material->GetTextureCount(aiTextureType_OPACITY))
    {
        aiString str;
        material->GetTexture(aiTextureType_OPACITY, 0, &str);
        std::string texturePath = GetDirectoryPath(msBasePath) + "textures/" + std::string(str.C_Str());
        internalMaterial->SetTexture(Material::OPACITY, Texture::Load(texturePath));
        LOG_INFO("AssimpLoader::ProcessMaterial::Texture::Opacity::Path: %s", texturePath.c_str());
    }

    
    Material::MaterialProperties materialProperties;
    aiColor3D color(0.f, 0.f, 0.f);

    // To get a material property. Inspect the AI_MATKEY_COLOR_DIFFUSE define to see other properties
    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
    {
        materialProperties.mDiffuse = { color.r, color.g, color.b };
    }

    //float shininess = 64.f;
    //if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
    //{
    //    materialProperties.mShininess = shininess;
    //}

    internalMaterial->SetProperties(materialProperties);

    return internalMaterial;
}

//AABB AssimpLoader::ProcessCollisionAABB(aiMesh* mesh)
//{
//    AABB aabb{ {},{} };
//    for (auto i = 0; i < mesh->mNumVertices; i++)
//    {
//        glm::vec3 pos{ 0.f };
//        pos.x = mesh->mVertices[i].x;
//        pos.y = mesh->mVertices[i].y;
//        pos.z = mesh->mVertices[i].z;
//
//        aabb.center += pos;
//    }
//    aabb.center /= mesh->mNumVertices;
//
//    for (auto i = 0; i < mesh->mNumVertices; i++)
//    {
//        glm::vec3 pos{ 0.f };
//        pos.x = mesh->mVertices[i].x;
//        pos.y = mesh->mVertices[i].y;
//        pos.z = mesh->mVertices[i].z;
//
//        LOG_INFO("aabb pos: (%f, %f, %f)", pos.x, pos.y, pos.z);
//        // Expands the AABB to fit the vertices
//        aabb.Expand(pos);
//    }
//
//    LOG_INFO("aabb extent: (%f, %f, %f)", aabb.extent.x, aabb.extent.y, aabb.extent.z);
//
//    return aabb;
//}