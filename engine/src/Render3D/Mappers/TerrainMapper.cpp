#include "TerrainMapper.hpp"

#include "InstancedModel.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "TextureLoader.hpp"

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace Engine {

std::shared_ptr<Model> TerrainMapper::map(const std::vector<float> &heightMap, unsigned int terrainWidth,
                                          unsigned int terrainHeight) {
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;

    for (unsigned int i = 0; i < terrainHeight; i++) {
        for (unsigned int j = 0; j < terrainWidth; j++) {
            float x = static_cast<float>(j);
            float z = static_cast<float>(i);

            glm::vec3 position = glm::vec3(x, 0.0f, z);
            glm::vec3 normal = glm::vec3(0.0f);
            glm::vec2 textCoord = glm::vec2(j, i);
            glm::vec3 tangent = glm::vec3(0.0f);
            glm::vec3 bitangent = glm::vec3(0.0f);
            glm::vec3 color = glm::vec3(0.5f);

            auto height = heightMap[static_cast<unsigned int>(i * terrainWidth + j)];

            vertices.emplace_back(position, normal, textCoord, tangent, bitangent, color);
        }
    }

    for (unsigned int i = 0; i < terrainHeight - 1; i++) {
        for (unsigned int j = 0; j < terrainWidth - 1; j++) {
            unsigned int v1 = (i + 1) * terrainWidth + j;
            unsigned int v2 = i * terrainWidth + j + 1;
            unsigned int v3 = i * terrainWidth + j;

            glm::vec3 edge1 = vertices[v2].position - vertices[v1].position;
            glm::vec3 edge2 = vertices[v3].position - vertices[v1].position;
            glm::vec2 deltaUV1 = vertices[v2].textCoord - vertices[v1].textCoord;
            glm::vec2 deltaUV2 = vertices[v3].textCoord - vertices[v1].textCoord;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            glm::vec3 tangent;
            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            glm::vec3 bitangent;
            bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
            bitangent = glm::normalize(bitangent);

            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            vertices[v1].tangent += tangent;
            vertices[v2].tangent += tangent;
            vertices[v3].tangent += tangent;

            vertices[v1].bitangent += bitangent;
            vertices[v2].bitangent += bitangent;
            vertices[v3].bitangent += bitangent;

            vertices[v1].normal += normal;
            vertices[v2].normal += normal;
            vertices[v3].normal += normal;

            indices.push_back(v1);
            indices.push_back(v2);
            indices.push_back(v3);

            v1 = (i + 1) * terrainWidth + j;
            v2 = (i + 1) * terrainWidth + j + 1;
            v3 = i * terrainWidth + j + 1;

            edge1 = vertices[v2].position - vertices[v1].position;
            edge2 = vertices[v3].position - vertices[v1].position;
            deltaUV1 = vertices[v2].textCoord - vertices[v1].textCoord;
            deltaUV2 = vertices[v3].textCoord - vertices[v1].textCoord;

            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);

            bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
            bitangent = glm::normalize(bitangent);

            normal = glm::normalize(glm::cross(edge1, edge2));

            vertices[v1].tangent += tangent;
            vertices[v2].tangent += tangent;
            vertices[v3].tangent += tangent;

            vertices[v1].bitangent += bitangent;
            vertices[v2].bitangent += bitangent;
            vertices[v3].bitangent += bitangent;

            vertices[v1].normal += normal;
            vertices[v2].normal += normal;
            vertices[v3].normal += normal;

            indices.push_back(v1);
            indices.push_back(v2);
            indices.push_back(v3);
        }
    }

    for (unsigned int i = 0; i < vertices.size(); i++) {
        vertices[i].tangent = glm::normalize(vertices[i].tangent);
        vertices[i].bitangent = glm::normalize(vertices[i].bitangent);
        vertices[i].normal = glm::normalize(vertices[i].normal);
    }

    // material.diffuseMap = TextureLoader::loadTexture("./assets/models/box/diffuse-map.jpeg");
    // material.specularMap = TextureLoader::loadTexture("./assets/models/box/specular-map.jpeg");
    // material.normalMap = TextureLoader::loadTexture("./assets/models/box/normal-map.jpeg");

    Mesh mesh(vertices, indices);
    return std::shared_ptr<Model>(new InstancedModel({mesh}));
}

} // namespace Engine