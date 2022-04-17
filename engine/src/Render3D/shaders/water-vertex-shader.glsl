#version 330 core

/////////////////////////////////////////////////////////////
//////////////////////// ATTRIBUTES /////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec2 a_vertexTextureCoord;

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;

uniform float u_density;
uniform float u_gradient;

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
out vec4 v_clipSpace;
out vec3 v_fragPos;
out vec2 v_texCoord;
out float v_visibility;

void main() {
    vec4 worldPos = u_model * vec4(a_vertexPosition, 1.0);
    vec4 positionRelativeToCamera = u_view * worldPos;

    v_clipSpace = u_projection * positionRelativeToCamera;
    v_texCoord = a_vertexTextureCoord;
    v_fragPos = vec3(worldPos);
    gl_Position = v_clipSpace;

    float distanceToCamera = length(positionRelativeToCamera.xyz);
    v_visibility = exp(-pow(distanceToCamera * u_density, u_gradient));
    v_visibility = clamp(v_visibility, 0.0, 1.0);
}