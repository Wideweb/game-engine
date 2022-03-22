#version 330 core

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const int c_kernelSize = 64;
const float c_radius = 0.5;
const float c_bias = 0.025;

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform sampler2D u_positionMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_noiseMap;

uniform vec3 u_samples[64];
uniform mat4 u_projection;
uniform mat4 u_view;
uniform vec2 u_noiseScale;

uniform int u_kernelSize = c_kernelSize;
uniform float u_radius = c_radius;
uniform float u_bias = c_bias;

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
in vec2 v_texCoord;

/////////////////////////////////////////////////////////////
/////////////////////////// OUT /////////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) out float o_occlusion;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    vec3 fragPos = (u_view * vec4(texture(u_positionMap, v_texCoord).rgb, 1.0)).xyz;
    vec3 fragNormal = texture(u_normalMap, v_texCoord).rgb;
    vec3 fragNoise = texture(u_noiseMap, v_texCoord * u_noiseScale).rgb;

    vec3 tangent = normalize(fragNoise - fragNormal * dot(fragNoise, fragNormal));
    vec3 bitangent = cross(fragNormal, tangent);
    mat3 TBN = mat3(tangent, bitangent, fragNormal);

    float occlusion = 0.0;
    for (int i = 0; i < u_kernelSize; ++i) {
        vec3 sample = TBN * u_samples[i];
        sample = fragPos + sample * u_radius;

        vec4 offset = vec4(sample, 1.0);
        offset = u_projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = (u_view * vec4(texture(u_positionMap, offset.xy).rgb, 1.0)).z;

        // occlusion += (sampleDepth >= fragPos.z + u_bias ? 1.0 : 0.0);

        float rangeCheck = smoothstep(0.0, 1.0, u_radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sample.z + u_bias ? 1.0 : 0.0) * rangeCheck;
    }

    o_occlusion = 1.0 - (occlusion / u_kernelSize);
}