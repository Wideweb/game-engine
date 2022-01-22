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
layout(location = 0) in int a_id;
layout(location = 1) in mat4 a_model;
layout(location = 5) in vec3 a_vertexPosition;
layout(location = 7) in vec2 a_vertexTextureCoord;
layout(location = 10) in vec3 a_vertexColor;
layout(location = 11) in ivec4 a_vertexJoints0;
layout(location = 12) in ivec4 a_vertexJoints1;
layout(location = 13) in vec4 a_vertexJointWeights0;
layout(location = 14) in vec4 a_vertexJointWeights1;
layout(location = 15) in int a_vertexJointsNumber;

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_joints[c_maxJoints];
uniform int u_jointsNumber;

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
flat out int v_id;
out vec3 v_color;
out vec2 v_texCoord;

flat out int g_id;
out vec3 g_color;
out vec2 g_texCoord;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    v_id = a_id;

    mat4 worldPos = a_model * getVertexTransform();
    vec4 positionRelativeToCamera = u_view * worldPos * vec4(a_vertexPosition, 1.0);

    gl_Position = u_projection * positionRelativeToCamera;
    v_color = a_vertexColor;
    v_texCoord = vec2(a_vertexTextureCoord.x, a_vertexTextureCoord.y);

    g_color = v_color;
    g_texCoord = v_texCoord;
    g_id = v_id;
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