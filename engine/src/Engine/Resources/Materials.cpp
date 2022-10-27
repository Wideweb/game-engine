#include "Materials.hpp"

#include <stdexcept>
#include <glm/vec3.hpp>

namespace Engine {

void Materials::init(const Shaders& shaders) {
    defaultMaterial = std::make_shared<Material>(shaders.defaultShader.get());
    m_Map.insert({"default", defaultMaterial});
    
    phongMaterial = std::make_shared<Material>(shaders.phongShader.get());
    m_Map.insert({"phong", phongMaterial});
    
    pbrMaterial = std::make_shared<Material>(shaders.pbrShader.get());
    m_Map.insert({"pbr", pbrMaterial});

    meshMaterial = std::make_shared<Material>(shaders.meshShader.get());
    meshMaterial->setDepthTest(false);
    meshMaterial->addFloat3("u_color", glm::vec3(0.0f, 1.0f, 0.0f));
    m_Map.insert({"mesh", meshMaterial});

    cubeEdgesMaterial = std::make_shared<Material>(shaders.cubeEdgesShader.get());
    cubeEdgesMaterial->setDepthTest(false);
    m_Map.insert({"cubeEdges", cubeEdgesMaterial});

    gridMaterial = std::make_shared<Material>(shaders.gridShader.get());
    m_Map.insert({"grid", gridMaterial});

    terrainBrushMaterial = std::make_shared<Material>(shaders.terrainBrushShader.get());
    terrainBrushMaterial->addFloat("u_radius");
    terrainBrushMaterial->addFloat2("u_mouse");
    m_Map.insert({"terrainBrush", terrainBrushMaterial});
}

void Materials::add(const std::string &name, Shader* shader) {
    if (m_Map.find(name) != m_Map.end()) {
        throw std::invalid_argument("material name is not unique.");
    }
    m_Map.insert({name, std::make_shared<Material>(shader)});
}

Material* Materials::get(const std::string &name) {
    return m_Map.at(name).get();
}

} // namespace Engine
