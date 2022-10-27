#include "Shaders.hpp"

#include "File.hpp"

#include <stdexcept>

namespace Engine {

void Shaders::init() {
    auto vertexSrc = File::read("./shaders/direct-vertex-shader.glsl");
    auto fragmentSrc = File::read("./shaders/direct-fragment-shader.glsl");
    defaultShader = std::make_shared<Shader>(vertexSrc, fragmentSrc);

    fragmentSrc = File::read("./shaders/direct-fragment-shader-spot-light.glsl");
    phongShader = std::make_shared<Shader>(vertexSrc, fragmentSrc);

    fragmentSrc = File::read("./shaders/direct-fragment-shader-pbr.glsl");
    pbrShader = std::make_shared<Shader>(vertexSrc, fragmentSrc);

    vertexSrc = File::read("./shaders/overlay-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/overlay-fragment-shader.glsl");
    auto geometrySrc = File::read("./shaders/overlay-geometry-shader.glsl");
    meshShader = std::make_shared<Shader>(vertexSrc, fragmentSrc, geometrySrc);

    vertexSrc = File::read("./shaders/overlay-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/overlay-fragment-shader.glsl");
    geometrySrc = File::read("./shaders/edge-geometry-shader.glsl");
    cubeEdgesShader = std::make_shared<Shader>(vertexSrc, fragmentSrc, geometrySrc);

    vertexSrc = File::read("./shaders/grid-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/grid-fragment-shader.glsl");
    gridShader = std::make_shared<Shader>(vertexSrc, fragmentSrc);

    vertexSrc = File::read("./shaders/brush-vertex-shader.glsl");
    fragmentSrc = File::read("./shaders/brush-fragment-shader.glsl");
    terrainBrushShader = std::make_shared<Shader>(vertexSrc, fragmentSrc);

    m_Map.insert({"default", defaultShader});
    m_Map.insert({"phong", phongShader});
    m_Map.insert({"pbr", pbrShader});
    m_Map.insert({"mesh", meshShader});
    m_Map.insert({"cubeEdges", cubeEdgesShader});
    m_Map.insert({"grid", gridShader});
    m_Map.insert({"terrainBrush", terrainBrushShader});
}

void Shaders::load(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc) {
    if (m_Map.find(name) != m_Map.end()) {
        throw std::invalid_argument("shader name is not unique.");
    }
    m_Map.insert({name, std::make_shared<Shader>(vertexSrc, fragmentSrc)});
}

Shader* Shaders::get(const std::string &name) {
    return m_Map.at(name).get();
}

} // namespace Engine
