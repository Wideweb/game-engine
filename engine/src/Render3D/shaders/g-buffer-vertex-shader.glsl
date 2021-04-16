#version 330 core

const int MAX_BONES = 100;

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTextureCoord;
layout(location = 3) in ivec4 VertexBones;
layout(location = 4) in vec4 VertexBoneWeights;
layout(location = 5) in int VertexBonesNumber;
layout(location = 6) in mat4 Model;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Bones[MAX_BONES];

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

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

    gl_Position =
        Projection * View * Model * transform * vec4(VertexPosition, 1.0);
    TexCoord = vec2(VertexTextureCoord.x, 1.0 - VertexTextureCoord.y);
    FragPos = vec3(Model * transform * vec4(VertexPosition, 1.0));
    Normal = mat3(transpose(inverse(Model * transform))) * VertexNormal;
}