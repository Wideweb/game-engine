#pragma once

#include "Mesh.hpp"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

#include <numeric>
#include <vector>

namespace Engine::Render3D {

class Utils {
  public:
    static void tbn(std::vector<Mesh::Vertex> &vertices, const std::vector<unsigned int> &indices) {
        for (unsigned int i = 0; i < indices.size() - 2; i += 3) {
            unsigned int v1 = indices[i];
            unsigned int v2 = indices[i + 1];
            unsigned int v3 = indices[i + 2];

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
    }

    static void tbn(std::vector<Mesh::Vertex> &vertices) {
        std::vector<unsigned int> indices(vertices.size());
        std::iota(indices.begin(), indices.end(), 0);
        tbn(vertices, indices);
    }
};

} // namespace Engine::Render3D
