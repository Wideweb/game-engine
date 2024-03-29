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
layout(location = 6) in vec3 a_vertexNormal;
layout(location = 7) in vec2 a_vertexTextureCoord;
layout(location = 8) in vec3 a_vertexTangent;
layout(location = 9) in vec3 a_vertexBitangent;
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
uniform int u_hasNormalMapping;

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
flat out int v_id;
out vec3 v_color;
out vec3 v_normal;
out vec2 v_texCoord;
out vec3 v_fragPos;
out vec3 v_fragCameraPos;
out vec2 v_fragScreenPos;
out mat3 v_TBN;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    v_id = a_id;

    mat4 modelMatrix = a_model * getVertexTransform();
    vec4 worldPosition = modelMatrix * vec4(a_vertexPosition, 1.0);

    v_color = a_vertexColor;
    v_texCoord = vec2(a_vertexTextureCoord.x, a_vertexTextureCoord.y);
    v_fragPos = vec3(worldPosition);
    v_fragCameraPos = vec3(u_view * worldPosition);
    v_normal = normalize(mat3(transpose(inverse(modelMatrix))) * a_vertexNormal);

    v_TBN = mat3(0);
    if (u_hasNormalMapping > 0) {
        vec3 T = normalize(vec3(modelMatrix * vec4(a_vertexTangent, 0.0)));
        vec3 N = normalize(vec3(modelMatrix * vec4(a_vertexNormal, 0.0)));

        T = normalize(T - dot(T, N) * N);
        vec3 B = cross(N, T);

        v_TBN = mat3(T, B, N);
    }

    vec4 screenPos = u_projection * vec4(v_fragCameraPos, 1.0);
    v_fragScreenPos = (screenPos / screenPos.w).xy;
    gl_Position = screenPos;
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