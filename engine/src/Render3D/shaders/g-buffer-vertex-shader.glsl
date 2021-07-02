#version 330 core

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
mat4 getVertexTransform();

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const int c_maxJoints = 100;
const float c_density = 0.03;
const float c_gradient = 5.0;

/////////////////////////////////////////////////////////////
//////////////////////// ATTRIBUTES /////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) in mat4 a_model;
layout(location = 4) in vec3 a_vertexPosition;
layout(location = 5) in vec3 a_vertexNormal;
layout(location = 6) in vec2 a_vertexTextureCoord;
layout(location = 7) in vec3 a_vertexTangent;
layout(location = 8) in vec3 a_vertexBitangent;
layout(location = 9) in vec3 a_vertexColor;
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
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
out vec3 v_color;
out vec3 v_normal;
out vec2 v_texCoord;
out vec3 v_fragPos;
out float v_visibility;
out mat3 v_TBN;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    mat4 worldPos = a_model * getVertexTransform();
    vec4 positionRelativeToCamera = u_view * worldPos * vec4(a_vertexPosition, 1.0);

    gl_Position = u_projection * positionRelativeToCamera;
    v_color = a_vertexColor;
    v_texCoord = vec2(a_vertexTextureCoord.x, 1.0 - a_vertexTextureCoord.y);
    v_fragPos = vec3(worldPos * vec4(a_vertexPosition, 1.0));
    v_normal = normalize(mat3(transpose(inverse(worldPos))) * a_vertexNormal);

    vec3 T = normalize(vec3(worldPos * vec4(a_vertexTangent, 0.0)));
    vec3 B = normalize(vec3(worldPos * vec4(a_vertexBitangent, 0.0)));
    vec3 N = normalize(vec3(worldPos * vec4(a_vertexNormal, 0.0)));
    v_TBN = mat3(T, B, N);

    float distanceToCamera = length(positionRelativeToCamera.xyz);
    v_visibility = exp(-pow(distanceToCamera * c_density, c_gradient));
    v_visibility = clamp(v_visibility, 0.0, 1.0);
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