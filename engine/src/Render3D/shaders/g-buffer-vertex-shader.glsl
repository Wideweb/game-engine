#version 330 core

const int MAX_JOINTS = 100;

layout(location = 0) in mat4 Model;
layout(location = 4) in vec3 VertexPosition;
layout(location = 5) in vec3 VertexNormal;
layout(location = 6) in vec2 VertexTextureCoord;
layout(location = 7) in ivec4 VertexJoints;
layout(location = 8) in vec4 VertexJointWeights;
layout(location = 9) in int VertexJointsNumber;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Joints[MAX_JOINTS];

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

mat4 getVertexTransform() {
    if (VertexJointsNumber <= 0) {
        return mat4(1.0);
    }

    mat4 transform = Joints[VertexJoints[0]] * VertexJointWeights[0];
    for (int i = 1; i < VertexJointsNumber; i++) {
        transform += Joints[VertexJoints[i]] * VertexJointWeights[i];
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