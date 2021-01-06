cmake_minimum_required(VERSION 3.13)

project(Engine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_LIB 
    vendor/glad/glad.c
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
    src/Engine/ECS/Systems/MoveSystem.cpp
    src/Engine/ECS/Systems/Light3DSystem.cpp
    src/Engine/ECS/Systems/Render3DSystem.cpp
    src/Engine/ECS/Systems/PhysicsSystem.cpp
    src/Engine/ECS/Systems/CollisionSystem.cpp
    src/Engine/ECS/Systems/AISystem.cpp
    src/Engine/ECS/Systems/CameraSystem.cpp
    src/Animation/AnimationLoader.cpp
    src/Render3D/Camera.cpp
    src/Render3D/Mesh.cpp
    src/Render3D/Model.cpp
    src/Render3D/ModelLoader.cpp
    src/Render3D/Render.cpp
    src/Render3D/Scene.cpp
    src/Render3D/Shader.cpp
    src/Render3D/Texture.cpp
    src/Collision/CollisionDetection.cpp
    src/Collision/BroadPhaseAlgorithm.cpp
    src/Collision/NarrowPhaseAlgorithm.cpp
    src/Collision/AABBOverlap.cpp
    src/Sound/SoundMixer.cpp
    src/Sound/SDL/SDLSoundBuffer.cpp
    src/Sound/SDL/SDLSoundMixer.cpp    
)	

add_library(engine SHARED ${SOURCE_LIB})

set_target_properties(engine PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/AI/BehaviourTree
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
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Sound
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Sound/SDL
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/ECS/Components
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/ECS/Systems
                                         ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/AI
                                         ${CMAKE_CURRENT_SOURCE_DIR}/vendor)


find_package(SDL2 REQUIRED)
target_include_directories(engine PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(engine PRIVATE ${SDL2_LIBRARIES})

find_package(glm REQUIRED)
target_link_libraries(engine PUBLIC glm)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
target_link_libraries(engine PRIVATE imgui)

target_link_libraries(engine PRIVATE -lprofiler)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/Render3D/shaders DESTINATION ${OUTPUT_DIRECTORY})