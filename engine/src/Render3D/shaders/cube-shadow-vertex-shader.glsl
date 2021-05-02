#version 330 core

const int MAX_JOINTS = 100;

layout(location = 0) in mat4 Model;
layout(location = 4) in vec3 VertexPosition;
layout(location = 7) in ivec4 VertexJoints0;
layout(location = 8) in ivec4 VertexJoints1;
layout(location = 9) in vec4 VertexJointWeights0;
layout(location = 10) in vec4 VertexJointWeights1;
layout(location = 11) in int VertexJointsNumber;

uniform mat4 Joints[MAX_JOINTS];
uniform int JointsNumber;

mat4 getVertexTransform() {
    if (VertexJointsNumber <= 0 || JointsNumber <= 0) {
        return mat4(1.0);
    }

    mat4 transform = Joints[VertexJoints0[0]] * VertexJointWeights0[0];
    for (int i = 1; i < VertexJointsNumber && i < 4; i++) {
        transform += Joints[VertexJoints0[i]] * VertexJointWeights0[i];
    }

    for (int i = 4; i < VertexJointsNumber && i < 8; i++) {
        transform += Joints[VertexJoints1[i]] * VertexJointWeights1[i];
    }

    return transform;
}

void main() {
    mat4 transform = getVertexTransform();
    gl_Position = Model * transform * vec4(VertexPosition, 1.0f);
}