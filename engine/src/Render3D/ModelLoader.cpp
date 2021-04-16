#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <utility>

#include "ModelLoader.hpp"
#include "TextureLoader.hpp"

namespace Engine {

AssimpModel::AssimpModel() {}

std::shared_ptr<Model> AssimpModel::load(std::string path) {
    m_Model = std::make_shared<Model>();

    Assimp::Importer import;
    const aiScene *scene =
        import.ReadFile(path, aiProcess_Triangulate | aiProcess_SortByPType);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return m_Model;
    }
    m_Directory = path.substr(0, path.find_last_of('/'));

    loadNode(scene->mRootNode, scene);

    for (unsigned int i = 0; i < m_Model->bones.size(); i++) {
        for (unsigned int j = i; j < m_Model->bones.size(); j++) {
            Bone &thisBone = m_Model->bones[i];
            Bone &thatBone = m_Model->bones[j];

            if (!thisBone.hasParent && thatBone.hasParent) {
                continue;
            }

            if ((thisBone.hasParent && !thatBone.hasParent) ||
                (thisBone.parentId > thatBone.parentId)) {
                m_BoneIndex[thisBone.name] = j;
                m_BoneIndex[thatBone.name] = j;

                std::swap(thisBone, thatBone);
            }
        }
    }

    buildSkelet(scene->mRootNode, scene);

    m_Model->animation.resize(m_Model->bones.size());

    aiAnimation *animationSrc = scene->mAnimations[0];

    for (unsigned int i = 0; i < animationSrc->mNumChannels; i++) {
        aiNodeAnim *nodeAnimation = animationSrc->mChannels[i];
        std::string nodeName = nodeAnimation->mNodeName.data;
        if (m_BoneIndex.find(nodeName) == m_BoneIndex.end()) {
            continue;
        }

        BoneAnimation &boneAnimation =
            m_Model->animation[m_BoneIndex[nodeName]];

        for (unsigned int j = 0; j < nodeAnimation->mNumPositionKeys; j++) {
            BoneAnimationKeyFrame frame;

            frame.timeStamp = nodeAnimation->mRotationKeys[j].mTime;
            frame.position =
                aiVector3DToGlm(nodeAnimation->mPositionKeys[j].mValue);
            frame.rotation =
                aiQuaternionDToGlm(nodeAnimation->mRotationKeys[j].mValue);

            boneAnimation.keyFrames.push_back(frame);
        }
    }

    m_Model->setUp();
    return m_Model;
}

void AssimpModel::loadNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_Model->meshes.push_back(loadMesh(mesh, scene));
        loadBones(mesh);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        loadNode(node->mChildren[i], scene);
    }
}

Mesh AssimpModel::loadMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;

        glm::vec3 normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;

        glm::vec2 textCoord;
        if (mesh->mTextureCoords[0]) {
            textCoord.x = mesh->mTextureCoords[0][i].x;
            textCoord.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.emplace_back(position, normal, textCoord);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *materialSrc = scene->mMaterials[mesh->mMaterialIndex];

        if (materialSrc->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        }

        // material.diffuseMap.reset(
        //     loadMaterialTexture(materialSrc, aiTextureType_DIFFUSE));
        // material.specularMap.reset(
        //     loadMaterialTexture(materialSrc, aiTextureType_SPECULAR));
        // material.normalMap.reset(
        //     loadMaterialTexture(materialSrc, aiTextureType_NORMALS));

        material.diffuseMap.reset(
            TextureLoader::loadTexture("./assets/models/box/diffuse-map.png"));
        material.specularMap.reset(
            TextureLoader::loadTexture("./assets/models/box/specular-map.png"));
        material.normalMap.reset(
            TextureLoader::loadTexture("./assets/models/box/normal-map.png"));
    }

    if (m_ActiveMeshes == 0) {
        m_MeshBaseVertex.push_back(0);
    } else {
        size_t base = m_Model->meshes[m_ActiveMeshes - 1].vertices.size();
        m_MeshBaseVertex.push_back(static_cast<unsigned int>(base));
    }

    m_MeshIndex[mesh->mName.data] = m_ActiveMeshes;
    m_ActiveMeshes++;

    return Mesh(vertices, indices, material);
}

Texture *AssimpModel::loadMaterialTexture(aiMaterial *materialSrc,
                                          aiTextureType type) {

    for (unsigned int i = 0; i < materialSrc->GetTextureCount(type); i++) {
        aiString str;
        materialSrc->GetTexture(type, i, &str);
        return TextureLoader::loadTexture(m_Directory +
                                          std::string(str.C_Str()));
    }
    return nullptr;
}

void AssimpModel::loadBones(aiMesh *mesh) {
    for (unsigned int i = 0; i < mesh->mNumBones; i++) {
        aiBone *bone = mesh->mBones[i];
        std::string boneName = bone->mName.data;

        unsigned int index = 0;
        const auto &boneIndexIt = m_BoneIndex.find(boneName);
        if (boneIndexIt == m_BoneIndex.end()) {
            index = static_cast<unsigned int>(m_Model->bones.size());
            m_Model->bones.emplace_back();
        } else {
            index = boneIndexIt->second;
        }

        m_BoneIndex[boneName] = index;
        m_Model->bones[index].offset = aiMatrix4x4ToGlm(bone->mOffsetMatrix);
        m_Model->bones[index].hasParent = false;
        m_Model->bones[index].name = boneName;
    }
}

void AssimpModel::buildSkelet(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        bindMeshToBone(mesh, scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        buildSkelet(node->mChildren[i], scene);
    }
}

void AssimpModel::bindMeshToBone(aiMesh *meshSrc, const aiScene *scene) {
    unsigned int meshIndex = m_MeshIndex[meshSrc->mName.data];
    auto &mesh = m_Model->meshes[meshIndex];
    unsigned int meshBaseVertex = m_MeshBaseVertex[meshIndex];

    for (unsigned int i = 0; i < meshSrc->mNumBones; i++) {
        aiBone *bone = meshSrc->mBones[i];
        std::string boneName = bone->mName.data;
        unsigned int index = m_BoneIndex.find(boneName)->second;

        aiNode *node = scene->mRootNode->FindNode(boneName.c_str());

        if (node->mParent &&
            m_BoneIndex.find(node->mParent->mName.data) != m_BoneIndex.end()) {
            m_Model->bones[index].parentId =
                m_BoneIndex[node->mParent->mName.data];
            m_Model->bones[index].hasParent = true;
        }

        for (unsigned int j = 0; j < bone->mNumWeights; j++) {
            unsigned int vertexId =
                bone->mWeights[j].mVertexId - meshBaseVertex;

            if (vertexId < 0 || vertexId >= mesh.vertices.size()) {
                continue;
            }

            auto &vertex = mesh.vertices[vertexId];
            float weight = bone->mWeights[j].mWeight;

            if (vertex.bonesNumber >= 4) {
                continue;
            }

            vertex.bones.ids[vertex.bonesNumber] = index;
            vertex.bones.weights[vertex.bonesNumber] = weight;
            vertex.bonesNumber++;
        }
    }
}

std::shared_ptr<Model> ModelLoader::loadModel(const std::string &path) {
    return AssimpModel().load(path);
}

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

    material.diffuseMap.reset(TextureLoader::loadTexture(toDiffuseMap));
    material.specularMap.reset(TextureLoader::loadTexture(toSpecularMap));
    material.normalMap.reset(TextureLoader::loadTexture(toNormalMap));

    Mesh mesh(vertices, indices, material);
    auto model = std::shared_ptr<Model>(new Model({mesh}));
    model->setUp();
    return model;
}

std::shared_ptr<Skybox>
ModelLoader::loadSkybox(const std::vector<std::string> &faces) {
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

    auto cubemapTexture =
        std::shared_ptr<Texture>(TextureLoader::loadCubemap(faces));
    return std::make_shared<Skybox>(vertices, cubemapTexture);
}

} // namespace Engine
