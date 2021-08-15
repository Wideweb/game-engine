#version 330 core

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const float c_lowerLimit = -1.0;
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
layout(location = 0) out vec4 o_fragColor;
layout(location = 1) out vec4 o_brightColor;

void main() {
    vec4 color = texture(u_skybox, v_texCoord);

    float factor = (v_texCoord.y - c_lowerLimit) / (c_upperLimit - c_lowerLimit);
    factor = clamp(factor, 0.0, 1.0);

    o_fragColor = mix(vec4(c_fogColor, 1.0), color, factor);

    float brightness = dot(o_fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0)
        o_brightColor = vec4(o_fragColor.rgb, 1.0);
    else
        o_brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
