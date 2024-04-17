# CMakeList.txt : CMake project for 3DCompulsoryFinal, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("3DCompulsoryFinal")

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
endif()

# Set C++ standard
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED True)
endif()

# Include directories
include_directories(
  core
  includes/imgui
  includes/glad
  includes/glad/KHR
  includes/stbi
  libs/glm
  libs/assimp/include
  #libs/eigen
)

#Add IMGUI headers
set( IMGUI_HEADERS
    includes/imgui/imgui.h
    includes/imgui/imconfig.h
    includes/imgui/imgui_impl_opengl3.h
    includes/imgui/imgui_impl_glfw.h
    includes/imgui/imgui_impl_opengl3_loader.h
)

#Add IMGUI headers
set( IMGUI_SOURCE
    includes/imgui/imgui.cpp
    includes/imgui/imgui_demo.cpp
    includes/imgui/imgui_draw.cpp
    includes/imgui/imgui_widgets.cpp
    includes/imgui/imgui_impl_opengl3.cpp
    includes/imgui/imgui_impl_glfw.cpp
    includes/imgui/imgui_tables.cpp
)

set (INCLUDES
includes/glad/glad.c
includes/stbi/stb_image.h
)

set (LIBS
)

set (HEADER_FILES
#Actor Headers
core/actors/Actor.h
core/actors/ActorController.h
core/actors/Scene.h
core/actors/SceneGraph.h
core/actors/PawnActor.h

#Camera Headers
core/camera/Camera.h
core/camera/CameraController.h

#Graphics Headers
core/graphics/Material.h
core/graphics/Mesh.h
core/graphics/Skybox.h
core/graphics/Texture.h
core/graphics/Vertex.h

#Interfaces Headers
core/interfaces/CollisionInterface.h
core/interfaces/ControllerInterface.h
core/interfaces/RenderInterface.h

#Lighting Headers
core/lighting/DirectionalLight.h
core/lighting/PointLight.h

#Mathematics Headers
core/mathematics/BarycentricC.h

#Physics Headers
core/physics/AABBcollision.h
core/physics/BarycentricCollision.h

#Program Headers
core/program/Application.h
core/program/Window.h

#Shader Headers
core/shaders/Shader.h

# Utility Headers
core/utility/AssimpLoader.h
core/utility/AssimpUtility.h
core/utility/Defines.h
core/utility/Logger.h
core/utility/Shared.h
core/utility/Tag.h
core/utility/Transform.h
core/utility/Types.h
)

set (SRC_FILES
3DCompulsoryFinal.cpp

#Actor Source
core/actors/Actor.cpp
core/actors/ActorController.cpp
core/actors/Scene.cpp
core/actors/PawnActor.cpp

#Camera Source
core/camera/Camera.cpp
core/camera/CameraController.cpp

#Graphics Source
core/graphics/Material.cpp
core/graphics/Mesh.cpp
core/graphics/Skybox.cpp
core/graphics/Texture.cpp

#Program Source
core/program/Application.cpp
core/program/Window.cpp

#Shader Source
core/shaders/Shader.cpp

#Utility Source
core/utility/AssimpLoader.cpp
)

add_subdirectory(libs/glfw-3.3.9)
add_subdirectory(libs/assimp)
add_subdirectory(libs/glm/glm)
add_subdirectory(libs/eigen)

# Add source to this project's executable.
add_executable (3DCompulsoryFinal 
${IMGUI_HEADERS} ${IMGUI_SOURCE} 
${INCLUDES} ${LIBS}
${HEADER_FILES} ${SRC_FILES}
)

target_link_libraries(${PROJECT_NAME} glfw assimp)

# MSVC specific settings
if(MSVC)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
