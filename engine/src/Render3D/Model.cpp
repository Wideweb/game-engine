#include "Model.hpp"

namespace Engine {

Model::Model() {}

Model::Model(const std::vector<Mesh> &meshes) : meshes(meshes) {}

Model::~Model() {}

void Model::setUp() {
    for (auto &mesh : meshes) {
        mesh.setUp();
    }
}

// glm::mat4 Model::BoneTransform(float timeInSeconds,
//                                vector<glm::mat4> &transforms) {
//     glm::mat4 transform = glm::mat4(1.0f);

//     float ticksPerSecond = m_pScene->mAnimations[0]->mTicksPerSecond != 0
//                                ? m_pScene->mAnimations[0]->mTicksPerSecond
//                                : 25.0f;

//     float timeInTicks = timeInSeconds * ticksPerSecond;
//     float animationTime =
//         fmod(timeInTicks, m_pScene->mAnimations[0]->mDuration);

//     ReadNodeHeirarchy(animationTime, m_pScene->mRootNode, transform);

//     Transforms.resize(m_NumBones);

//     for (uint i = 0; i < m_NumBones; i++) {
//         Transforms[i] = m_BoneInfo[i].FinalTransformation;
//     }
// }

// void Model::ReadNodeHeirarchy(float animationTime, const aiNode *node,
//                               const Matrix4f &parentTransform) {
//     std::string nodeName = node->mName.data;

//     const aiAnimation *pAnimation = m_pScene->mAnimations[0];

//     glm::mat4 transform = node->mTransformation;

//     const aiNodeAnim *pNodeAnim = FindNodeAnim(pAnimation, NodeName);

//     if (pNodeAnim) {
//         // Interpolate scaling and generate scaling transformation matrix
//         aiVector3D Scaling;
//         CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
//         Matrix4f ScalingM;
//         ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

//         // Interpolate rotation and generate rotation transformation matrix
//         aiQuaternion RotationQ;
//         CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
//         Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

//         // Interpolate translation and generate translation transformation
//         // matrix
//         aiVector3D Translation;
//         CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
//         Matrix4f TranslationM;
//         TranslationM.InitTranslationTransform(Translation.x, Translation.y,
//                                               Translation.z);

//         // Combine the above transformations
//         NodeTransformation = TranslationM * RotationM * ScalingM;
//     }

//     Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;

//     if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
//         uint BoneIndex = m_BoneMapping[NodeName];
//         m_BoneInfo[BoneIndex].FinalTransformation =
//             m_GlobalInverseTransform * GlobalTransformation *
//             m_BoneInfo[BoneIndex].BoneOffset;
//     }

//     for (uint i = 0; i < pNode->mNumChildren; i++) {
//         ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i],
//                           GlobalTransformation);
//     }
// }

// const aiNodeAnim *SkinnedMesh::FindNodeAnim(const aiAnimation *pAnimation,
//                                             const string NodeName) {
//     for (uint i = 0; i < pAnimation->mNumChannels; i++) {
//         const aiNodeAnim *pNodeAnim = pAnimation->mChannels[i];

//         if (string(pNodeAnim->mNodeName.data) == NodeName) {
//             return pNodeAnim;
//         }
//     }

//     return NULL;
// }

// void Mesh::CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTime,
//                                     const aiNodeAnim *pNodeAnim) {
//     // для интерполирования требуется не менее 2 значений...
//     if (pNodeAnim->mNumRotationKeys == 1) {
//         Out = pNodeAnim->mRotationKeys[0].mValue;
//         return;
//     }

//     uint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
//     uint NextRotationIndex = (RotationIndex + 1);

//     float DeltaTime = pNodeAnim->mRotationKeys[NextRotationIndex].mTime -
//                       pNodeAnim->mRotationKeys[RotationIndex].mTime;
//     float Factor =
//         (AnimationTime -
//         (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;

//     const aiQuaternion &StartRotationQ =
//         pNodeAnim->mRotationKeys[RotationIndex].mValue;
//     const aiQuaternion &EndRotationQ =
//         pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
//     aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
//     Out = Out.Normalize();
// }

} // namespace Engine
