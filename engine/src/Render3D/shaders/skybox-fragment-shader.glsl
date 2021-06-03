#version 330 core

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const float c_lowerLimit = 0.0;
const float c_upperLimit = 1.0;

const vec3 c_fogColor = vec3(0.55, 0.69, 0.73);

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform samplerCube u_skybox;

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
in vec3 v_texCoord;

/////////////////////////////////////////////////////////////
/////////////////////////// OUT /////////////////////////////
/////////////////////////////////////////////////////////////
out vec4 o_fragColor;

void main() {
    vec4 color = texture(u_skybox, v_texCoord);

    float factor = (v_texCoord.y - c_lowerLimit) / (c_upperLimit - c_lowerLimit);
    factor = clamp(factor, 0.0, 1.0);

    o_fragColor = mix(vec4(c_fogColor, 1.0), color, factor);
}
