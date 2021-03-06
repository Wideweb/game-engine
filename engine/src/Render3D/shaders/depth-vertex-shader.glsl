#version 330 core

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
mat4 getVertexTransform();

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const int c_maxJoints = 100;

/////////////////////////////////////////////////////////////
//////////////////////// ATTRIBUTES /////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) in mat4 a_model;
layout(location = 4) in vec3 a_vertexPosition;
layout(location = 10) in ivec4 a_vertexJoints0;
layout(location = 11) in ivec4 a_vertexJoints1;
layout(location = 12) in vec4 a_vertexJointWeights0;
layout(location = 13) in vec4 a_vertexJointWeights1;
layout(location = 14) in int a_vertexJointsNumber;

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_joints[c_maxJoints];
uniform int u_jointsNumber;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    mat4 transform = getVertexTransform();
    gl_Position = u_projection * u_view * a_model * transform * vec4(a_vertexPosition, 1.0f);
}

mat4 getVertexTransform() {
    if (a_vertexJointsNumber <= 0 || u_jointsNumber <= 0) {
        return mat4(1.0);
    }

    mat4 transform = u_joints[a_vertexJoints0[0]] * a_vertexJointWeights0[0];
    for (int i = 1; i < a_vertexJointsNumber && i < 4; i++) {
        transform += u_joints[a_vertexJoints0[i]] * a_vertexJointWeights0[i];
    }

    for (int i = 4; i < a_vertexJointsNumber && i < 8; i++) {
        transform += u_joints[a_vertexJoints1[i]] * a_vertexJointWeights1[i];
    }

    return transform;
}