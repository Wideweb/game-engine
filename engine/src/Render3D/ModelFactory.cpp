#include "ModelFactory.hpp"

#include "Material.hpp"
#include "Mesh.hpp"

#include <glm/vec3.hpp>

namespace Engine {

std::shared_ptr<Model> ModelFactory::createCube(float size) {
    // clang-format off
    std::vector<glm::vec3> positions = {
        glm::vec3(-1.0f, 1.0f, 1.0f),
        glm::vec3(-1.0f, -1.0f, 1.0f),
        glm::vec3(-1.0f, 1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
    };

    for (unsigned int i = 0; i < positions.size(); i++) {
        positions[i] *= size;
    }

    std::vector<glm::vec2> textCoord = {
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
    };

    std::vector<Mesh::Vertex> vertices;
    vertices.emplace_back(positions[4], glm::vec3(0.0f), textCoord[0], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[2], glm::vec3(0.0f), textCoord[0], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[0], glm::vec3(0.0f), textCoord[0], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    
    vertices.emplace_back(positions[2], glm::vec3(0.0f), textCoord[1], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[7], glm::vec3(0.0f), textCoord[1], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[3], glm::vec3(0.0f), textCoord[1], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    
    vertices.emplace_back(positions[6], glm::vec3(0.0f), textCoord[2], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[5], glm::vec3(0.0f), textCoord[2], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[7], glm::vec3(0.0f), textCoord[2], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    
    vertices.emplace_back(positions[1], glm::vec3(0.0f), textCoord[3], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[7], glm::vec3(0.0f), textCoord[3], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[5], glm::vec3(0.0f), textCoord[3], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[0], glm::vec3(0.0f), textCoord[4], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[3], glm::vec3(0.0f), textCoord[4], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[1], glm::vec3(0.0f), textCoord[4], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[4], glm::vec3(0.0f), textCoord[5], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[1], glm::vec3(0.0f), textCoord[5], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[5], glm::vec3(0.0f), textCoord[5], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[4], glm::vec3(0.0f), textCoord[0], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[6], glm::vec3(0.0f), textCoord[0], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[2], glm::vec3(0.0f), textCoord[0], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[2], glm::vec3(0.0f), textCoord[1], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[6], glm::vec3(0.0f), textCoord[1], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[7], glm::vec3(0.0f), textCoord[1], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[6], glm::vec3(0.0f), textCoord[2], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[4], glm::vec3(0.0f), textCoord[2], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[5], glm::vec3(0.0f), textCoord[2], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[1], glm::vec3(0.0f), textCoord[3], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[3], glm::vec3(0.0f), textCoord[3], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[7], glm::vec3(0.0f), textCoord[3], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[0], glm::vec3(0.0f), textCoord[4], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[2], glm::vec3(0.0f), textCoord[4], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[3], glm::vec3(0.0f), textCoord[4], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));

    vertices.emplace_back(positions[4], glm::vec3(0.0f), textCoord[5], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[0], glm::vec3(0.0f), textCoord[5], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    vertices.emplace_back(positions[1], glm::vec3(0.0f), textCoord[5], glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.6f, 0.1f));
    // clang-format on

    std::vector<GLuint> indices;
    Material material;

    for (unsigned int i = 0; i < vertices.size(); i += 3) {
        unsigned int v1 = i;
        unsigned int v2 = i + 1;
        unsigned int v3 = i + 2;

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
    }

    for (unsigned int i = 0; i < vertices.size(); i++) {
        vertices[i].tangent = glm::normalize(vertices[i].tangent);
        vertices[i].bitangent = glm::normalize(vertices[i].bitangent);
        vertices[i].normal = glm::normalize(vertices[i].normal);
    }

    Mesh mesh(vertices, indices, material);
    mesh.hasMaterial = false;
    auto model = std::shared_ptr<Model>(new InstancedModel({mesh}));
    model->setUp();
    return model;
}

std::shared_ptr<InstancedModel> ModelFactory::createPlane(float tileSize, int columns, int rows) {
    // clang-format off
    glm::vec2 center;
    center.x = columns * tileSize / 2.0f;
    center.y = rows * tileSize / 2.0f;

    std::vector<Mesh::Vertex> vertices;
    for (int i = 0; i <= rows; i++) {
        for (int j = 0; j <= columns; j++) {
            float x = static_cast<float>(j);
            float z = static_cast<float>(i);

            vertices.emplace_back(
                glm::vec3(x * tileSize - center.x, 0.0f, z * tileSize - center.y), 
                glm::vec3(0.0f),
                glm::vec2(x, z),
                glm::vec3(0.0f),
                glm::vec3(0.0f),
                glm::vec3(0.8f, 0.6f, 0.1f)
            );
        }
    }
    // clang-format on

    std::vector<GLuint> indices;
    Material material;

    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < columns; j++) {
            unsigned int v1 = (i + 1) * (columns + 1) + j;
            unsigned int v2 = i * (columns + 1) + j + 1;
            unsigned int v3 = i * (columns + 1) + j;

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

            v1 = (i + 1) * (columns + 1) + j;
            v2 = (i + 1) * (columns + 1) + j + 1;
            v3 = i * (columns + 1) + j + 1;

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

    Mesh mesh(vertices, indices, material);
    mesh.hasMaterial = false;
    auto model = std::shared_ptr<InstancedModel>(new InstancedModel({mesh}));
    model->setUp();
    return model;
}

} // namespace Engine
