#version 330 core
in vec4 FragPos;

uniform vec3 CubePosition;
uniform float FarPlane;

void main() { gl_FragDepth = length(FragPos.xyz - CubePosition) / FarPlane; }