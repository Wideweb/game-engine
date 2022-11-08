#include "ModelMapper.hpp"

#include "InstancedModel.hpp"
#include "PrimitivesMapper.hpp"
#include "TextureLoader.hpp"

#include <filesystem>

namespace Engine {

std::shared_ptr<Model> ModelMapper::map(const aiScene *scene) {
    m_Model = std::make_shared<SkinnedModel>();

    m_GlobalInverseTransform = PrimitivesMapper::map(scene->mRootNode->mTransformation.Inverse());

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
                frame.position = PrimitivesMapper::map(nodeAnimation->mPositionKeys[j].mValue);
                frame.rotation = PrimitivesMapper::map(nodeAnimation->mRotationKeys[j].mValue);
                frame.scale = PrimitivesMapper::map(nodeAnimation->mScalingKeys[j].mValue);

                jointAnimation.keyFrames.push_back(frame);
            }
        }
    }

    if (!m_Skelet.joints.empty()) {
        m_Model->skelet = std::move(m_Skelet);
        return m_Model;
    }

    auto model = std::make_shared<Model3D>();
    model->meshes.resize(m_Model->meshes.size());
    for (size_t i = 0; i < m_Model->meshes.size(); i++) {
        auto &mesh = model->meshes[i];
        auto &meshSrc = m_Model->meshes[i];

        mesh.indices = meshSrc.indices;
        // mesh.material = meshSrc.material;

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

    return model;
}

void ModelMapper::loadNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_Model->meshes.push_back(loadMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        loadNode(node->mChildren[i], scene);
    }
}

SkinnedMesh ModelMapper::loadMesh(aiMesh *meshSrc, const aiScene *scene) {
    std::vector<SkinnedMesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    SkinnedMesh mesh;

    for (unsigned int i = 0; i < meshSrc->mNumVertices; i++) {
        glm::vec3 position = PrimitivesMapper::map(meshSrc->mVertices[i]);
        glm::vec3 normal = PrimitivesMapper::map(meshSrc->mNormals[i]);
        glm::vec3 tangent = PrimitivesMapper::map(meshSrc->mTangents[i]);
        glm::vec3 bitangent = PrimitivesMapper::map(meshSrc->mBitangents[i]);
        glm::vec3 color = glm::vec3(0.0f);

        if (meshSrc->mColors[0] != nullptr) {
            color = glm::vec3(PrimitivesMapper::map(meshSrc->mColors[0][i]));
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

        // mesh.material.diffuseMap = loadMaterialTexture(materialSrc, aiTextureType_DIFFUSE);
        // mesh.material.specularMap = loadMaterialTexture(materialSrc, aiTextureType_SPECULAR);
        // mesh.material.normalMap = loadMaterialTexture(materialSrc, aiTextureType_NORMALS);
        // mesh.hasMaterial = true;
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

Texture ModelMapper::loadMaterialTexture(aiMaterial *materialSrc, aiTextureType type) {
    for (unsigned int i = 0; i < materialSrc->GetTextureCount(type); i++) {
        aiString str;
        materialSrc->GetTexture(type, i, &str);
        if (std::filesystem::exists(str.C_Str())) {
            return TextureLoader::loadTexture(std::string(str.C_Str()));
        }
    }
    return Texture::getEmpty();
}

void ModelMapper::addJoint(aiBone *joint) {
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
    m_Skelet.joints[index].offset = PrimitivesMapper::map(joint->mOffsetMatrix);
    m_Skelet.joints[index].parentId = -1;
    m_Skelet.joints[index].name = name;
}

void ModelMapper::sortJoints() {
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

void ModelMapper::bindSkelet(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        bindMeshToSkelet(mesh);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        bindSkelet(node->mChildren[i], scene);
    }
}

void ModelMapper::bindMeshToSkelet(aiMesh *meshSrc) {
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

} // namespace Engine