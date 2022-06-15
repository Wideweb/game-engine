cmake_minimum_required(VERSION 3.13)

project(EngineApp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(ENV{CONAN_SYSREQUIRES_MODE} enabled)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_PREFIX_PATH})

set(SOURCE_EXE 
    vendor/glad/glad.c
    vendor/imgui_impl_opengl3.cpp
    vendor/imgui_impl_sdl.cpp
    src/main.cpp
    src/AppLayer.cpp
    src/Core/Time.cpp
    src/Core/File.cpp
    src/Core/Math.cpp
    src/IO/Window.cpp
    src/IO/Input.cpp
    src/IO/SDL/SDLWindow.cpp
    src/IO/SDL/SDLInput.cpp
    src/Engine/Application.cpp
    src/Engine/Layer.cpp
    src/Engine/TextureManager.cpp
    src/Engine/FontManager.cpp
    src/Engine/MousePicker.cpp
    src/Engine/ECS/Systems/MoveSystem.cpp
    src/Engine/ECS/Systems/LocationSystem.cpp
    src/Engine/ECS/Systems/Location2DSystem.cpp
    src/Engine/ECS/Systems/DirectedLightSystem.cpp
    src/Engine/ECS/Systems/SpotLightSystem.cpp
    src/Engine/ECS/Systems/Render3DSystem.cpp
    src/Engine/ECS/Systems/SkeletSystem.cpp
    src/Engine/ECS/Systems/StaticRender3DSystem.cpp
    src/Engine/ECS/Systems/PhysicsSystem.cpp
    src/Engine/ECS/Systems/CollisionSystem.cpp
    src/Engine/ECS/Systems/TerrainCollisionSystem.cpp
    src/Engine/ECS/Systems/StaticCollisionSystem.cpp
    src/Engine/ECS/Systems/AISystem.cpp
    src/Engine/ECS/Systems/CameraSystem.cpp
    src/Engine/ECS/Systems/BehaviourSystem.cpp
    src/Engine/ECS/Systems/ParticlesSystem.cpp
    src/Engine/ECS/Systems/ParentSystem.cpp
    src/Engine/ECS/Systems/Text2DSystem.cpp
    src/Engine/ECS/Systems/DestroySystem.cpp
    src/Engine/Scripting/Context/LuaCore.cpp
    src/Engine/Scripting/Context/LuaEntity.cpp
    src/Engine/Scripting/Context/LuaInput.cpp
    src/Engine/Scripting/Context/LuaRender.cpp
    src/Engine/Scripting/Context/LuaLayer.cpp
    src/Engine/Scripting/Scripts/EntityScript.cpp
    src/Engine/Scripting/Scripts/EntryScript.cpp
    src/Engine/EditTools/ImguiImpl.cpp
    src/Engine/EditTools/EditToolsLayer.cpp
    src/Engine/EditTools/Components/BaseView.cpp
    src/Engine/EditTools/Components/ToolbarPanel.cpp
    src/Engine/EditTools/Components/RenderPanel.cpp
    src/Engine/EditTools/Components/ModelRenderPanel.cpp
    src/Engine/EditTools/Components/Transform2DPanel.cpp
    src/Engine/EditTools/Components/TransformPanel.cpp
    src/Engine/EditTools/Components/TransformControlsPosition.cpp
    src/Engine/EditTools/Components/TransformControlsRotation.cpp
    src/Engine/EditTools/Components/TransformControlsScale.cpp
    src/Engine/EditTools/Components/DirectedLightPanel.cpp
    src/Engine/EditTools/Components/SpotLightPanel.cpp
    src/Engine/EditTools/Components/SpotLightSceneTool.cpp
    src/Engine/EditTools/Components/ParticlesPanel.cpp
    src/Engine/EditTools/Components/SceneHierarchyPanel.cpp
    src/Engine/EditTools/Components/ContentBrowserPanel.cpp
    src/Engine/EditTools/Components/BehaviourPanel.cpp
    src/Engine/EditTools/Components/CameraDirector.cpp
    src/Engine/EditTools/Components/ScenePanel.cpp
    src/Engine/EditTools/Components/GamePanel.cpp
    src/Engine/EditTools/Components/ConsolePanel.cpp
    src/Engine/EditTools/Components/InspectorPanel.cpp
    src/Engine/EditTools/Components/VelocityPanel.cpp
    src/Engine/EditTools/Components/DirectedLightDirector.cpp
    src/Engine/EditTools/Components/SkeletPanel.cpp
    src/Engine/EditTools/Components/CollisionBody.cpp
    src/Engine/EditTools/Components/RigitBodyPanel.cpp
    src/Engine/EditTools/Components/CollisionPanel.cpp
    src/Engine/EditTools/Components/TerrainPanel.cpp
    src/Engine/EditTools/Components/TerrainTransform.cpp
    src/Engine/EditTools/Components/MeshBody.cpp
    src/Engine/EditTools/Components/SkyboxPanel.cpp
    src/Engine/EditTools/Components/Text2DPanel.cpp
    src/Engine/EditTools/Components/CameraPanel.cpp
    src/ECS/EntityManager.cpp
    src/Animation/AnimationLoader.cpp
    src/Render3D/Camera.cpp
    src/Render3D/CameraController.cpp
    src/Render3D/Models/Mesh.cpp
    src/Render3D/Models/InstancedModel.cpp
    src/Render3D/Models/SkinnedMesh.cpp
    src/Render3D/Models/SkinnedModel.cpp
    src/Render3D/Models/Particles.cpp
    src/Render3D/Models/Skybox.cpp
    src/Render3D/Mappers/ModelMapper.cpp
    src/Render3D/Mappers/TerrainMapper.cpp
    src/Render3D/Renderers/MasterRenderer.cpp
    src/Render3D/Renderers/QuadRenderer.cpp
    src/Render3D/Renderers/ModelRenderer.cpp
    src/Render3D/Renderers/OverlayRenderer.cpp
    src/Render3D/Renderers/SkyboxRenderer.cpp
    src/Render3D/Renderers/DirectedLightRenderer.cpp
    src/Render3D/Renderers/SpotLightRenderer.cpp
    src/Render3D/Renderers/WaterRenderer.cpp
    src/Render3D/Renderers/FlareRenderer.cpp
    src/Render3D/Renderers/FontRenderer.cpp
    src/Render3D/Renderers/GRenderer.cpp
    src/Render3D/Renderers/ParticlesRenderer.cpp
    src/Render3D/Renderers/Renderer2D.cpp
    src/Render3D/Renderers/DeferredRenderer.cpp
    src/Render3D/GfxObjects/GfxUtils.cpp
    src/Render3D/GfxObjects/GfxImage.cpp
    src/Render3D/GfxObjects/Texture.cpp
    src/Render3D/GfxObjects/Renderbuffer.cpp
    src/Render3D/GfxObjects/Framebuffer.cpp
    src/Render3D/GfxObjects/Shader.cpp
    src/Render3D/ModelLoader.cpp
    src/Render3D/ModelFactory.cpp
    src/Render3D/Viewport.cpp
    src/Render3D/Scene.cpp
    src/Render3D/TextureLoader.cpp
    src/Render3D/CubeMap.cpp
    src/Render3D/Font.cpp
    src/Collision/NarrowPhaseAlgorithm.cpp
    src/Collision/AABBOverlap.cpp
    src/Sound/SoundMixer.cpp
    src/Sound/SDL/SDLSoundBuffer.cpp
    src/Sound/SDL/SDLSoundMixer.cpp    
)

add_executable(${PROJECT_NAME} ${SOURCE_EXE})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/AI/BehaviourTree
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/AI/BehaviourTree/Tasks
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/AI/BehaviourTree/TaskDecorators
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Animation
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Collision
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Collision/Shapes
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Core
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/ECS
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/IO
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/IO/SDL
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Render3D
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Render3D/GfxObjects
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Render3D/Models
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Render3D/Mappers
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Render3D/Renderers
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Render3D/Utils
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Sound
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Sound/SDL
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/Scripting
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/Scripting/Context
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/Scripting/Scripts
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/ECS/Components
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/ECS/Systems
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/AI
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/EditTools
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/EditTools/Components
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/EditTools/Models
                                         ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES sdl/2.0.14 assimp/5.0.1 glm/0.9.9.8 lua/5.4.1 freetype/2.11.1 zlib/1.2.12
                      GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})

find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)

find_package(glm REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

find_package(assimp REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)

find_package(lua REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE lua::lua)

find_package(Freetype REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Freetype::Freetype)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

# target_link_libraries(${PROJECT_NAME} PRIVATE -lprofiler)
# target_link_libraries(${PROJECT_NAME} PRIVATE -ltcmalloc)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/Render3D/shaders DESTINATION ${OUTPUT_DIRECTORY})