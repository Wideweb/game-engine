#version 330 core

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform sampler2D u_colorMap;
uniform float u_alpha;

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
in vec2 v_texCoord;
in vec4 v_color;

/////////////////////////////////////////////////////////////
//////////////////////////// OUT ////////////////////////////
/////////////////////////////////////////////////////////////
out vec4 o_fragColor;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    o_fragColor = v_color * texture(u_colorMap, v_texCoord);
    // FragColor.a = FragColor.a * alpha;
}
