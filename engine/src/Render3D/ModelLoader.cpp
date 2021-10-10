#include <filesystem>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <utility>

#include "InstancedMesh.hpp"
#include "InstancedModel.hpp"
#include "Material.hpp"
#include "ModelLoader.hpp"
#include "TextureLoader.hpp"

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

AssimpModel::AssimpModel() {}

std::shared_ptr<Model> AssimpModel::load(std::string path) {
    m_Model = std::make_shared<SkinnedModel>();

    Assimp::Importer import;
    const aiScene *scene =
        import.ReadFile(path, aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph |
                                  aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return m_Model;
    }
    m_Directory = path.substr(0, path.find_last_of('/'));

    m_GlobalInverseTransform = aiMatrix4x4ToGlm(scene->mRootNode->mTransformation.Inverse());

    loadNode(scene->mRootNode, scene);

    for (auto const &[name, index] : m_JointIndex) {
        aiNode *node = scene->mRootNode->FindNode(name.c_str());

        if (!node || !node->mParent) {
            continue;
        }

        auto parentIt = m_JointIndex.end();
        while (node->mParent && parentIt == m_JointIndex.end()) {
            parentIt = m_JointIndex.find(node->mParent->mName.data);
            node = node->mParent;
        }

        if (parentIt == m_JointIndex.end()) {
            continue;
        }

        m_Skelet.joints[index].parentId = static_cast<int>(parentIt->second);
    }

    sortJoints();
    bindSkelet(scene->mRootNode, scene);

    for (unsigned j = 0; j < scene->mNumAnimations; j++) {
        aiAnimation *animationSrc = scene->mAnimations[j];
        auto &animation = m_Skelet.animations[animationSrc->mName.data];
        animation.joints.resize(m_Skelet.joints.size());
        animation.duration = static_cast<float>(animationSrc->mDuration);
        animation.ticksPerSecond =
            animationSrc->mTicksPerSecond != 0.0 ? static_cast<float>(animationSrc->mTicksPerSecond) : 25.0f;

        for (unsigned int i = 0; i < animationSrc->mNumChannels; i++) {
            aiNodeAnim *nodeAnimation = animationSrc->mChannels[i];
            std::string nodeName = nodeAnimation->mNodeName.data;
            if (m_JointIndex.find(nodeName) == m_JointIndex.end()) {
                continue;
            }

            auto &jointAnimation = animation.joints[m_JointIndex[nodeName]];

            for (unsigned int j = 0; j < nodeAnimation->mNumPositionKeys; j++) {
                JointAnimationKeyFrame frame;

                frame.timeStamp = static_cast<float>(nodeAnimation->mRotationKeys[j].mTime);
                frame.position = aiVector3DToGlm(nodeAnimation->mPositionKeys[j].mValue);
                frame.rotation = aiQuaternionDToGlm(nodeAnimation->mRotationKeys[j].mValue);
                frame.scale = aiVector3DToGlm(nodeAnimation->mScalingKeys[j].mValue);

                jointAnimation.keyFrames.push_back(frame);
            }
        }
    }

    if (!m_Skelet.joints.empty()) {
        m_Model->skelet = std::move(m_Skelet);
        m_Model->setUp();
        return m_Model;
    }

    auto model = std::make_shared<InstancedModel>();
    model->meshes.resize(m_Model->meshes.size());
    for (size_t i = 0; i < m_Model->meshes.size(); i++) {
        auto &mesh = model->meshes[i];
        auto &meshSrc = m_Model->meshes[i];

        mesh.hasMaterial = meshSrc.hasMaterial;
        mesh.indices = meshSrc.indices;
        mesh.material = meshSrc.material;

        mesh.vertices.resize(meshSrc.vertices.size());
        for (size_t j = 0; j < meshSrc.vertices.size(); j++) {
            auto &vertex = mesh.vertices[j];
            auto &vertexSrc = meshSrc.vertices[j];

            vertex.position = vertexSrc.position;
            vertex.normal = vertexSrc.normal;
            vertex.textCoord = vertexSrc.textCoord;
            vertex.tangent = vertexSrc.tangent;
            vertex.bitangent = vertexSrc.bitangent;
            vertex.color = vertexSrc.color;
        }
    }

    model->setUp();
    return model;
}

void AssimpModel::loadNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_Model->meshes.push_back(loadMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        loadNode(node->mChildren[i], scene);
    }
}

SkinnedMesh AssimpModel::loadMesh(aiMesh *meshSrc, const aiScene *scene) {
    std::vector<SkinnedMesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    SkinnedMesh mesh;

    for (unsigned int i = 0; i < meshSrc->mNumVertices; i++) {
        glm::vec3 position = aiVector3DToGlm(meshSrc->mVertices[i]);
        glm::vec3 normal = aiVector3DToGlm(meshSrc->mNormals[i]);
        glm::vec3 tangent = aiVector3DToGlm(meshSrc->mTangents[i]);
        glm::vec3 bitangent = aiVector3DToGlm(meshSrc->mBitangents[i]);
        glm::vec3 color = glm::vec3(0.0f);

        if (meshSrc->mColors[0] != nullptr) {
            color = glm::vec3(aiColorToGlm(meshSrc->mColors[0][i]));
        }

        glm::vec2 textCoord;
        if (meshSrc->mTextureCoords[0]) {
            textCoord.x = meshSrc->mTextureCoords[0][i].x;
            textCoord.y = meshSrc->mTextureCoords[0][i].y;
        }

        vertices.emplace_back(position, normal, textCoord, tangent, bitangent, color);
    }
    mesh.vertices = vertices;

    for (unsigned int i = 0; i < meshSrc->mNumFaces; i++) {
        aiFace face = meshSrc->mFaces[i];
        assert(face.mNumIndices == 3);
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    mesh.indices = indices;

    if (meshSrc->mMaterialIndex >= 0) {
        aiMaterial *materialSrc = scene->mMaterials[meshSrc->mMaterialIndex];

        // if (materialSrc->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        //     mesh.material.diffuseMap.reset(TextureLoader::loadTexture("./assets/models/box/diffuse-map.jpeg"));
        //     mesh.material.specularMap.reset(TextureLoader::loadTexture("./assets/models/box/specular-map.jpeg"));
        //     mesh.material.normalMap.reset(TextureLoader::loadTexture("./assets/models/box/normal-map.jpeg"));
        //     mesh.hasMaterial = true;
        // }

        mesh.material.diffuseMap.reset(loadMaterialTexture(materialSrc, aiTextureType_DIFFUSE));
        mesh.material.specularMap.reset(loadMaterialTexture(materialSrc, aiTextureType_SPECULAR));
        mesh.material.normalMap.reset(loadMaterialTexture(materialSrc, aiTextureType_NORMALS));
        mesh.hasMaterial = true;
    }

    for (unsigned int i = 0; i < meshSrc->mNumBones; i++) {
        addJoint(meshSrc->mBones[i]);
    }

    if (m_ActiveMeshes == 0) {
        m_MeshBaseVertex.push_back(0);
    } else {
        size_t base =
            m_MeshBaseVertex[m_MeshBaseVertex.size() - 1] + m_Model->meshes[m_ActiveMeshes - 1].vertices.size();
        m_MeshBaseVertex.push_back(static_cast<unsigned int>(base));
    }

    m_MeshIndex[meshSrc->mName.data] = m_ActiveMeshes;
    m_ActiveMeshes++;

    return mesh;
}

Texture *AssimpModel::loadMaterialTexture(aiMaterial *materialSrc, aiTextureType type) {
    for (unsigned int i = 0; i < materialSrc->GetTextureCount(type); i++) {
        aiString str;
        materialSrc->GetTexture(type, i, &str);
        if (std::filesystem::exists(str.C_Str())) {
            return TextureLoader::loadTexture(std::string(str.C_Str()));
        }
    }
    return TextureLoader::placeholder();
}

void AssimpModel::addJoint(aiBone *joint) {
    std::string name = joint->mName.data;
    unsigned int index = 0;
    const auto &jointIndexIt = m_JointIndex.find(name);
    if (jointIndexIt == m_JointIndex.end()) {
        index = static_cast<unsigned int>(m_Skelet.joints.size());
        m_Skelet.joints.emplace_back();
    } else {
        index = jointIndexIt->second;
    }

    m_JointIndex[name] = index;
    m_Skelet.joints[index].offset = aiMatrix4x4ToGlm(joint->mOffsetMatrix);
    m_Skelet.joints[index].parentId = -1;
    m_Skelet.joints[index].name = name;
}

void AssimpModel::sortJoints() {
    if (m_Skelet.joints.empty()) {
        return;
    }

    std::vector<Joint> sortedJoints;
    std::unordered_map<std::string, unsigned int> sortedJointIndex;

    for (unsigned int i = 0; i < m_Skelet.joints.size(); i++) {
        if (m_Skelet.joints[i].parentId == -1) {
            sortedJointIndex[m_Skelet.joints[i].name] = static_cast<unsigned int>(sortedJoints.size());
            sortedJoints.push_back(m_Skelet.joints[i]);
        }
    }

    for (unsigned int i = 0; i < m_Skelet.joints.size(); i++) {
        int oldParentId = static_cast<int>(m_JointIndex[sortedJoints[i].name]);
        int newParentId = static_cast<int>(i);

        for (unsigned int j = 0; j < m_Skelet.joints.size(); j++) {
            if (m_Skelet.joints[j].parentId == oldParentId) {
                Joint joint = m_Skelet.joints[j];
                joint.parentId = newParentId;

                sortedJointIndex[joint.name] = static_cast<unsigned int>(sortedJoints.size());
                sortedJoints.push_back(joint);
            }
        }
    }

    m_Skelet.joints = std::move(sortedJoints);
    m_JointIndex = std::move(sortedJointIndex);

    // for (size_t i = 0; i < m_Skelet.joints.size(); i++) {
    //     std::cout << m_Skelet.joints[i].name << std::endl;
    // }
}

void AssimpModel::bindSkelet(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        bindMeshToSkelet(mesh);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        bindSkelet(node->mChildren[i], scene);
    }
}

void AssimpModel::bindMeshToSkelet(aiMesh *meshSrc) {
    unsigned int meshIndex = m_MeshIndex[meshSrc->mName.data];
    auto &mesh = m_Model->meshes[meshIndex];
    unsigned int meshBaseVertex = m_MeshBaseVertex[meshIndex];

    for (unsigned int i = 0; i < meshSrc->mNumBones; i++) {
        aiBone *joint = meshSrc->mBones[i];
        std::string jointName = joint->mName.data;
        unsigned int index = m_JointIndex.find(jointName)->second;

        for (unsigned int j = 0; j < joint->mNumWeights; j++) {
            unsigned int vertexId = joint->mWeights[j].mVertexId - meshBaseVertex;

            if (vertexId < 0 || vertexId >= mesh.vertices.size()) {
                continue;
            }

            auto &vertex = mesh.vertices[vertexId];
            float weight = joint->mWeights[j].mWeight;

            if (vertex.jointsNumber >= 8) {
                continue;
            }

            vertex.joints.ids[vertex.jointsNumber] = index;
            vertex.joints.weights[vertex.jointsNumber] = weight;
            vertex.jointsNumber++;
        }
    }
}

std::shared_ptr<Model> ModelLoader::loadModel(const std::string &path) {
    return std::static_pointer_cast<Model>(AssimpModel().load(path));
}

std::shared_ptr<Model> ModelLoader::load(const std::string &toObj, const std::string &toDiffuseMap,
                                         const std::string &toSpecularMap, const std::string &toNormalMap) {
    std::ifstream in(toObj, std::ios::in | std::ios::binary);
    std::stringstream dto;
    std::string line;

    std::string attribute;
    std::vector<glm::vec3> pVertices;
    std::vector<glm::vec3> nVertices;
    std::vector<glm::vec2> tVertices;
    std::vector<InstancedMesh::Vertex> vertices;
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

    material.diffuseMap.reset(TextureLoader::loadTexture(toDiffuseMap));
    material.specularMap.reset(TextureLoader::loadTexture(toSpecularMap));
    material.normalMap.reset(TextureLoader::loadTexture(toNormalMap));

    InstancedMesh mesh(vertices, indices, material);
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

    auto cubemapTexture = std::shared_ptr<Texture>(TextureLoader::loadCubemap(faces));
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
    unsigned char *heightMapSrc = stbi_load(path.data(), &width, &height, &nrChannels, 0);
    StbiPixel *heightMap = reinterpret_cast<StbiPixel *>(heightMapSrc);

    std::vector<InstancedMesh::Vertex> vertices;
    std::vector<GLuint> indices;
    Material material;

    const unsigned int uWidth = static_cast<unsigned int>(width);
    const unsigned int uHeight = static_cast<unsigned int>(height);

    const float rateHeight = uHeight * 1.0f / terrainHeight;
    const float rateWidth = uWidth * 1.0f / terrainWidth;

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

            auto height = heightMap[static_cast<unsigned int>(i * rateHeight * uWidth + j * rateWidth)];

            position.y = (height.r + height.g + height.b) / 3.0f / 255.0f * maxHeight;

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

    material.diffuseMap.reset(TextureLoader::loadTexture("./assets/models/box/diffuse-map.jpeg"));
    material.specularMap.reset(TextureLoader::loadTexture("./assets/models/box/specular-map.jpeg"));
    material.normalMap.reset(TextureLoader::loadTexture("./assets/models/box/normal-map.jpeg"));

    InstancedMesh mesh(vertices, indices, material);
    auto model = std::shared_ptr<Model>(new InstancedModel({mesh}));
    model->setUp();
    return model;
}

} // namespace Engine
