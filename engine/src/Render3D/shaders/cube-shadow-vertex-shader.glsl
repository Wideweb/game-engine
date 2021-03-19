#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 3) in mat4 Model;

void main() { gl_Position = Model * vec4(VertexPosition, 1.0f); }