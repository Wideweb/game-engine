#version 330 core

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
in vec4 v_fragPos;

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform vec3 u_cubePosition;
uniform float u_farPlane;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() { gl_FragDepth = length(v_fragPos.xyz - u_cubePosition) / u_farPlane; }