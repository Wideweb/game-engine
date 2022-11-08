#version 330 core

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
const int c_linearToneMapping = 0;
const int c_simpleReinhardToneMapping = 1;
const int c_lumaBasedReinhardToneMapping = 2;
const int c_whitePreservingLumaBasedReinhardToneMapping = 3;
const int c_filmicToneMapping = 4;
const int c_romBinDaHouseToneMapping = 5;
const int c_uncharted2ToneMapping = 6;

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform sampler2D u_hdrBuffer;
uniform sampler2D u_blurBuffer;
uniform usampler2D u_id;
uniform sampler2D u_exposure;
uniform int u_toneMapping;

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
in vec2 v_texCoord;

/////////////////////////////////////////////////////////////
/////////////////////////// OUT /////////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) out vec4 o_fragColor;
layout(location = 1) out uint o_id;

vec3 linearToneMapping(vec3 color, float exposure) {
    color = clamp(exposure * color, 0.0, 1.0);
    return color;
}

vec3 simpleReinhardToneMapping(vec3 color, float exposure) {
    color *= exposure / (1.0 + color / exposure);
    return color;
}

vec3 lumaBasedReinhardToneMapping(vec3 color) {
    float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
    float toneMappedLuma = luma / (1. + luma);
    color *= toneMappedLuma / luma;
    return color;
}

vec3 whitePreservingLumaBasedReinhardToneMapping(vec3 color) {
    float white = 2.;
    float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
    float toneMappedLuma = luma * (1. + luma / (white * white)) / (1. + luma);
    color *= toneMappedLuma / luma;
    return color;
}

vec3 filmicToneMapping(vec3 color) {
    color = max(vec3(0.), color - vec3(0.004));
    color = (color * (6.2 * color + .5)) / (color * (6.2 * color + 1.7) + 0.06);
    return color;
}

vec3 romBinDaHouseToneMapping(vec3 color) {
    color = exp(-1.0 / (2.72 * color + 0.15));
    return color;
}

vec3 uncharted2ToneMapping(vec3 color, float exposure) {
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;
    float W = 11.2;
    color *= exposure;
    color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - E / F;
    float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
    color /= white;
    return color;
}

vec3 toneMapping(vec3 color, float exposure) {
    switch (u_toneMapping) {
    case c_linearToneMapping:
        return linearToneMapping(color, exposure);
    case c_simpleReinhardToneMapping:
        return simpleReinhardToneMapping(color, exposure);
    case c_lumaBasedReinhardToneMapping:
        return lumaBasedReinhardToneMapping(color);
    case c_whitePreservingLumaBasedReinhardToneMapping:
        return whitePreservingLumaBasedReinhardToneMapping(color);
    case c_filmicToneMapping:
        return filmicToneMapping(color);
    case c_romBinDaHouseToneMapping:
        return romBinDaHouseToneMapping(color);
    case c_uncharted2ToneMapping:
        return uncharted2ToneMapping(color, exposure);
    }
}

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    o_id = texture(u_id, v_texCoord).r;

    vec3 hdrColor = texture(u_hdrBuffer, v_texCoord).rgb;
    vec3 blurColor = texture(u_blurBuffer, v_texCoord).rgb;
    hdrColor += blurColor;

    float exposure = texture(u_exposure, vec2(0.5)).r;
    o_fragColor = vec4(toneMapping(hdrColor, exposure), 1.0);
}
