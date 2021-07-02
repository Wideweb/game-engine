#version 330 core

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform int u_hasMaterial;
uniform Material u_material;

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
in vec3 v_color;
in vec2 v_texCoord;

/////////////////////////////////////////////////////////////
//////////////////////////// OUT ////////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) out vec4 o_fragColor;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    vec3 color = v_color;
    if (u_hasMaterial > 0) {
        color = texture(u_material.specular, v_texCoord).rgb;
    }

    o_fragColor = vec4(color, 1.0);
}