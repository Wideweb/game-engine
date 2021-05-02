#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "Model.hpp"
#include "Skelet.hpp"
#include "SkinnedMesh.hpp"
#include "SkinnedModel.hpp"
#include "Skybox.hpp"

namespace Engine {

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from) {
    glm::mat4 to;

    to[0][0] = static_cast<GLfloat>(from.a1);
    to[0][1] = static_cast<GLfloat>(from.b1);
    to[0][2] = static_cast<GLfloat>(from.c1);
    to[0][3] = static_cast<GLfloat>(from.d1);
    to[1][0] = static_cast<GLfloat>(from.a2);
    to[1][1] = static_cast<GLfloat>(from.b2);
    to[1][2] = static_cast<GLfloat>(from.c2);
    to[1][3] = static_cast<GLfloat>(from.d2);
    to[2][0] = static_cast<GLfloat>(from.a3);
    to[2][1] = static_cast<GLfloat>(from.b3);
    to[2][2] = static_cast<GLfloat>(from.c3);
    to[2][3] = static_cast<GLfloat>(from.d3);
    to[3][0] = static_cast<GLfloat>(from.a4);
    to[3][1] = static_cast<GLfloat>(from.b4);
    to[3][2] = static_cast<GLfloat>(from.c4);
    to[3][3] = static_cast<GLfloat>(from.d4);

    return to;
}

inline glm::vec3 aiVector3DToGlm(const aiVector3D &from) {
    glm::vec3 to;

    to.x = static_cast<GLfloat>(from.x);
    to.y = static_cast<GLfloat>(from.y);
    to.z = static_cast<GLfloat>(from.z);

    return to;
}

inline glm::quat aiQuaternionDToGlm(const aiQuaternion &from) {
    glm::quat to;

    to.x = static_cast<GLfloat>(from.x);
    to.y = static_cast<GLfloat>(from.y);
    to.z = static_cast<GLfloat>(from.z);
    to.w = static_cast<GLfloat>(from.w);

    return to;
}

class AssimpModel {
  private:
    std::shared_ptr<SkinnedModel> m_Model;
    Skelet m_Skelet;
    std::string m_Directory;
    std::unordered_map<std::string, unsigned int> m_JointIndex;
    std::unordered_map<std::string, unsigned int> m_MeshIndex;
    unsigned int m_ActiveMeshes = 0;
    std::vector<unsigned int> m_MeshBaseVertex;
    glm::mat4 m_GlobalInverseTransform;

  public:
    AssimpModel();
    std::shared_ptr<SkinnedModel> load(std::string path);

  private:
    void loadNode(aiNode *node, const aiScene *scene);
    SkinnedMesh loadMesh(aiMesh *mesh, const aiScene *scene);
    void addJoint(aiBone *joint);
    void sortJoints();
    void bindSkelet(aiNode *node, const aiScene *scene);
    void bindMeshToSkelet(aiMesh *mesh);
    Texture *loadMaterialTexture(aiMaterial *materialSrc, aiTextureType type);
};

class ModelLoader {
  public:
    static std::shared_ptr<Model> loadModel(const std::string &path);

    static std::shared_ptr<Model> load(const std::string &toObj,
                                       const std::string &toDiffuseMap,
                                       const std::string &toSpecularMap,
                                       const std::string &toNormalMap);

    static std::shared_ptr<Skybox>
    loadSkybox(const std::vector<std::string> &faces);

    static std::shared_ptr<Model> loadTerrain(const std::string &path,
                                              unsigned int terrainWidth,
                                              unsigned int terrainHeight,
                                              float maxHeight);
};

} // namespace Engine
