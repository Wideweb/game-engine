#version 330 core

const int MAX_BONES = 100;

layout(location = 0) in vec3 VertexPosition;
layout(location = 3) in ivec4 VertexBones;
layout(location = 4) in vec4 VertexBoneWeights;
layout(location = 5) in int VertexBonesNumber;
layout(location = 6) in mat4 Model;

uniform mat4 Bones[MAX_BONES];

mat4 getVertexTransform() {
    if (VertexBonesNumber <= 0) {
        return mat4(1.0);
    }

    mat4 transform = Bones[VertexBones[0]] * VertexBoneWeights[0];
    for (int i = 1; i < VertexBonesNumber; i++) {
        transform += Bones[VertexBones[i]] * VertexBoneWeights[i];
    }

    return transform;
}

void main() {
    mat4 transform = getVertexTransform();
    gl_Position = Model * transform * vec4(VertexPosition, 1.0f);
}