# CMakeList.txt : CMake project for 3DCompulsoryFinal, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("3DCompulsoryFinal")

# Add the directory containing GLFW's header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes/imgui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/core)

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
includes/glad.c
includes/stb_image.h
)

set (LIBS

)

set (HEADER_FILES
3DCompulsoryFinal.h

#Actor Headers
core/actors/Scene.h
core/actors/SceneGraph.h
core/actors/Actor.h

#Camera Headers
core/camera/Camera.h
core/camera/CameraController.h

#Graphics Headers
core/graphics/Texture.h
core/graphics/Material.h
core/graphics/Vertex.h
core/graphics/Mesh.h

#Interfaces Headers
core/interfaces/ControllerInterface.h

#Program Headers
core/program/Application.h
core/program/Window.h

#Shader Headers
core/shaders/Shader.h

# Utility Headers
core/utility/Logger.h
core/utility/Transform.h
core/utility/Defines.h
core/utility/Tag.h
core/utility/Types.h
)

set (SRC_FILES
3DCompulsoryFinal.cpp

#Actor Source
core/actors/Scene.cpp
core/actors/Actor.cpp

#Camera Source
core/camera/Camera.cpp
core/camera/CameraController.cpp

#Graphics Source
core/graphics/Texture.cpp
core/graphics/Material.cpp
core/graphics/Mesh.cpp

#Program Source
core/program/Application.cpp
core/program/Window.cpp

#Shader Source
core/shaders/Shader.cpp
)

# Add source to this project's executable.
add_executable (3DCompulsoryFinal
${IMGUI_HEADERS} ${IMGUI_SOURCE} 
${INCLUDES} ${LIBS}
${HEADER_FILES} ${SRC_FILES}
)

#target_link_libraries
target_link_libraries(3DCompulsoryFinal PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw3.lib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 3DCompulsoryFinal PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
