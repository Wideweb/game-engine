#include <fstream>
#include <glm/vec3.hpp>
#include <iostream>

#include "ModelLoader.hpp"

namespace Engine {

std::shared_ptr<Model> ModelLoader::load(const std::string &toObj,
                                         const std::string &toDiffuseMap,
                                         const std::string &toSpecularMap,
                                         const std::string &toNormalMap) {
    std::ifstream in(toObj, std::ios::in | std::ios::binary);
    std::stringstream dto;
    std::string line;

    std::string attribute;
    std::vector<glm::vec3> pVertices;
    std::vector<glm::vec3> nVertices;
    std::vector<glm::vec2> tVertices;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    Material material;

    while (!in.eof() && in >> attribute) {
        if (attribute == "o") {
        } else if (attribute == "v") {
            float x, y, z;
            in >> x >> y >> z;
            pVertices.emplace_back(x, y, z);
        } else if (attribute == "vt") {
            float x, y;
            in >> x >> y;
            tVertices.emplace_back(x, y);
        } else if (attribute == "vn") {
            float x, y, z;
            in >> x >> y >> z;
            nVertices.emplace_back(x, y, z);
        } else if (attribute == "f") {
            char divider;
            size_t p, t, n;
            in >> p >> divider >> t >> divider >> n;
            vertices.emplace_back(pVertices[p - 1], nVertices[n - 1],
                                  tVertices[t - 1]);
            in >> p >> divider >> t >> divider >> n;
            vertices.emplace_back(pVertices[p - 1], nVertices[n - 1],
                                  tVertices[t - 1]);
            in >> p >> divider >> t >> divider >> n;
            vertices.emplace_back(pVertices[p - 1], nVertices[n - 1],
                                  tVertices[t - 1]);
        } else {
            std::getline(in, line);
        }
    }

    in.close();

    material.diffuseMap.reset(new Texture(toDiffuseMap));
    material.specularMap.reset(new Texture(toSpecularMap));
    material.normalMap.reset(new Texture(toNormalMap));

    Mesh mesh(vertices, indices, material);
    return std::shared_ptr<Model>(new Model({mesh}));
}

} // namespace Engine
