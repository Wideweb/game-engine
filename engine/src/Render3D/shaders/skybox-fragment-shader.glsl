#version 330 core

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform samplerCube u_skybox;
uniform float u_threshold;
uniform vec4 u_clipPlane;
uniform vec3 u_fogColor;
uniform float u_lowerLimit;
uniform float u_upperLimit;

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
in vec3 v_texCoord;
in vec4 v_fragPos;

/////////////////////////////////////////////////////////////
/////////////////////////// OUT /////////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) out vec4 o_fragColor;
layout(location = 1) out int o_id;
layout(location = 2) out vec4 o_brightColor;

void main() {
    o_id = 0;

    if (dot(v_fragPos, u_clipPlane) < 0) {
        discard;
    }

    vec4 color = texture(u_skybox, v_texCoord);

    float factor = (v_texCoord.y - u_lowerLimit) / (u_upperLimit - u_lowerLimit);
    factor = clamp(factor, 0.0, 1.0);

    o_fragColor = mix(vec4(u_fogColor, 1.0), color, factor);

    float brightness = dot(o_fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > u_threshold)
        o_brightColor = vec4(o_fragColor.rgb, 1.0);
    else
        o_brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
