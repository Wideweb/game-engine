#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <sstream>
#include <utility>

#include "InstancedModel.hpp"
#include "Mesh.hpp"
#include "ModelLoader.hpp"
#include "TextureLoader.hpp"

#include "ModelMapper.hpp"
#include "TerrainMapper.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wcast-align"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wimplicit-int-conversion"
#include "stb_image.hpp"
#pragma GCC diagnostic pop

namespace Engine {

std::shared_ptr<Model> ModelLoader::load(const std::string &path) {
    Assimp::Importer import;
    const aiScene *scene =
        import.ReadFile(path, aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph |
                                  aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return std::make_shared<SkinnedModel>();
    }

    auto model = ModelMapper().map(scene);
    model->setUp();
    return model;
}

std::shared_ptr<Model> ModelLoader::loadObj(const std::string &path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    std::stringstream dto;
    std::string line;

    std::string attribute;
    std::vector<glm::vec3> pVertices;
    std::vector<glm::vec3> nVertices;
    std::vector<glm::vec2> tVertices;
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;

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
            vertices.emplace_back(pVertices[p - 1], nVertices[n - 1], tVertices[t - 1], glm::vec3(1.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f));
            in >> p >> divider >> t >> divider >> n;
            vertices.emplace_back(pVertices[p - 1], nVertices[n - 1], tVertices[t - 1], glm::vec3(1.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f));
            in >> p >> divider >> t >> divider >> n;
            vertices.emplace_back(pVertices[p - 1], nVertices[n - 1], tVertices[t - 1], glm::vec3(1.0f, 0.0f, 0.0f),
                                  glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f));
        } else {
            std::getline(in, line);
        }
    }

    in.close();

    Mesh mesh(vertices, indices);
    auto model = std::shared_ptr<Model>(new InstancedModel({mesh}));
    model->setUp();
    return model;
}

std::shared_ptr<Skybox> ModelLoader::loadSkybox(const std::vector<std::string> &faces) {
    // clang-format off
    std::vector<SkyboxVertex> vertices = {
        SkyboxVertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(1.0f,  1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(-1.0f,  1.0f, -1.0f)),

        SkyboxVertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(-1.0f, -1.0f,  1.0f)),

        SkyboxVertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(1.0f, -1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(1.0f,  1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(1.0f, -1.0f, -1.0f)),

        SkyboxVertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(1.0f, -1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(-1.0f, -1.0f,  1.0f)),

        SkyboxVertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(1.0f,  1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(-1.0f,  1.0f, -1.0f)),

        SkyboxVertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        SkyboxVertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
        SkyboxVertex(glm::vec3(1.0f, -1.0f,  1.0f))
    };
    // clang-format on

    auto cubemapTexture = TextureLoader::loadCubemap(faces);
    return std::make_shared<Skybox>(vertices, cubemapTexture);
}

std::shared_ptr<Model> ModelLoader::loadTerrain(const std::string &path, unsigned int terrainWidth,
                                                unsigned int terrainHeight, float maxHeight) {
    struct StbiPixel {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    int width, height, nrChannels;
    auto *heightMapSrc = reinterpret_cast<StbiPixel *>(stbi_load(path.data(), &width, &height, &nrChannels, 0));
    std::vector<float> heightMap;

    const float rateHeight = height * 1.0f / terrainHeight;
    const float rateWidth = width * 1.0f / terrainWidth;

    for (unsigned int i = 0; i < terrainHeight; i++) {
        for (unsigned int j = 0; j < terrainWidth; j++) {
            float x = static_cast<float>(j);
            float z = static_cast<float>(i);

            auto &pixel = heightMapSrc[static_cast<unsigned int>(i * rateHeight * width + j * rateWidth)];
            heightMap.push_back((pixel.r + pixel.g + pixel.b) / 3.0f / 255.0f * maxHeight);
        }
    }

    auto model = TerrainMapper().map(heightMap, terrainWidth, terrainHeight);
    model->setUp();
    return model;
}

} // namespace Engine
