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
uniform vec3 u_viewPos;

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
out vec4 v_clipSpace;
out vec2 v_texCoord;
out vec3 v_viewDir;

void main() {
    vec4 worldPos = u_model * vec4(a_vertexPosition, 1.0);
    v_clipSpace = u_projection * u_view * worldPos;
    v_texCoord = a_vertexTextureCoord;
    v_viewDir = u_viewPos - worldPos.xyz;
    gl_Position = v_clipSpace;
}