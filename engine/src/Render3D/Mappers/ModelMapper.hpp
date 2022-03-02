#pragma once

#include "Model.hpp"
#include "Skelet.hpp"
#include "SkinnedModel.hpp"
#include "Texture.hpp"

#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/texture.h>

#include <glm/mat4x4.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Engine {

class ModelMapper {
  private:
    std::shared_ptr<SkinnedModel> m_Model;
    Skelet m_Skelet;
    std::unordered_map<std::string, unsigned int> m_JointIndex;
    std::unordered_map<std::string, unsigned int> m_MeshIndex;
    unsigned int m_ActiveMeshes = 0;
    std::vector<unsigned int> m_MeshBaseVertex;
    glm::mat4 m_GlobalInverseTransform;

    void loadNode(aiNode *node, const aiScene *scene);
    SkinnedMesh loadMesh(aiMesh *mesh, const aiScene *scene);
    void addJoint(aiBone *joint);
    void sortJoints();
    void bindSkelet(aiNode *node, const aiScene *scene);
    void bindMeshToSkelet(aiMesh *mesh);
    Texture loadMaterialTexture(aiMaterial *materialSrc, aiTextureType type);

  public:
    std::shared_ptr<Model> map(const aiScene *scene);
};

} // namespace Engine