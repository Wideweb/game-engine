#version 330 core

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
struct DirectedLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const float c_waveStrength = 0.02;
const float c_near = 0.01;
const float c_far = 100.0;
const vec3 c_fogColor = vec3(0.55, 0.69, 0.73);

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform mat4 u_noramlFix;
uniform sampler2D u_colorMap;
uniform sampler2D u_depthMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_positionMap;
uniform sampler2D u_dudvMap;
uniform float u_moveFactor;
uniform DirectedLight u_directedLight;
uniform vec3 u_viewPos;
uniform float u_threshold;

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
in vec4 v_clipSpace;
in vec3 v_fragPos;
in vec2 v_texCoord;
in float v_visibility;

/////////////////////////////////////////////////////////////
/////////////////////////// OUT /////////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) out vec4 o_fragColor;
layout(location = 2) out vec4 o_brightColor;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    vec2 ndc = (v_clipSpace.xy / v_clipSpace.w) / 2.0 + 0.5;

    vec2 reflectTexCoord = vec2(ndc.x, -ndc.y);

    float depth = texture(u_depthMap, reflectTexCoord).r;
    float floorDistance = 2.0 * c_near * c_far / (c_far + c_near - depth * (c_far - c_near));
    float waterDistance = 2.0 * c_near * c_far / (c_far + c_near - gl_FragCoord.z * (c_far - c_near));
    float waterDepth = 0; // floorDistance - waterDistance;

    vec2 texCoordDistorted = texture(u_dudvMap, vec2(v_texCoord.x + u_moveFactor, v_texCoord.y)).rg * 0.1;
    texCoordDistorted = v_texCoord + vec2(texCoordDistorted.x, texCoordDistorted.y + u_moveFactor);
    vec2 totalDistortion = (texture(u_dudvMap, texCoordDistorted).rg * 2.0 - 1.0) * c_waveStrength;

    // vec2 distortion1 =
    //     (texture(u_dudvMap, vec2(v_texCoord.x + u_moveFactor, v_texCoord.y)).rg * 2.0 - 1.0) * c_waveStrength;
    // vec2 distortion2 =
    //     (texture(u_dudvMap, vec2(-v_texCoord.x + u_moveFactor, v_texCoord.y + u_moveFactor)).rg * 2.0 - 1.0) *
    //     c_waveStrength;

    // vec2 totalDistortion = distortion1 + distortion2;

    reflectTexCoord += totalDistortion;
    reflectTexCoord.x = clamp(reflectTexCoord.x, 0.001, 0.999);
    reflectTexCoord.y = clamp(reflectTexCoord.y, -0.999, -0.001);

    vec3 viewDir = normalize(u_viewPos - v_fragPos);
    vec3 reflectDir = normalize(v_fragPos - texture(u_positionMap, reflectTexCoord).rgb);

    float refractFactor = dot(normalize(viewDir), vec3(0, 1, 0));
    refractFactor = pow(refractFactor, 2.0);

    vec4 reflectColor = vec4(texture(u_colorMap, reflectTexCoord).rgb, 1.0 - refractFactor);
    reflectColor = mix(reflectColor, vec4(0.0, 1.0, 1.0, reflectColor.a), 0.5);

    vec3 normalMapColor = texture(u_normalMap, texCoordDistorted).rgb;
    vec3 normal = normalize(mat3(u_noramlFix) * normalize(normalMapColor * 2.0 - 1.0));

    vec3 halfwayDir = normalize(viewDir - u_directedLight.direction);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), 128);
    vec3 specular = u_directedLight.specular * specularFactor;

    o_fragColor = vec4(u_directedLight.ambient, 1.0) * reflectColor + vec4(specular, 0.0);
    // o_fragColor.a = min(o_fragColor.a, clamp(waterDepth / 5.0, 0.0, 1.0));

    o_fragColor = mix(vec4(c_fogColor, 1.0), o_fragColor, v_visibility);

    float brightness = dot(o_fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > u_threshold)
        o_brightColor = vec4(o_fragColor.rgb, 1.0);
    else
        o_brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
