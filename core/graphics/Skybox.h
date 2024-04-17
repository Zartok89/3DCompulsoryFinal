#pragma once

#include "utility/Types.h"
#include <initializer_list>
#include <string>


class Skybox
{
public:
	/*
	 * Constructor + Deconstructor
	 */
	Skybox(std::initializer_list<std::string> texturePaths);
	~Skybox();

	void RenderSkybox(class Camera* renderCamera);

	/*
	 * Member variables
	 */
	TextureID mTextureId{0};
	class Mesh* mMesh{ nullptr };
	class Shader* mSkyboxShader{ nullptr };
};
