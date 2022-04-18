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
const vec4 c_waterColor = vec4(0.0, 0.3, 0.5, 1.0);

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform mat4 u_noramlFix;
uniform sampler2D u_reflectionMap;
uniform sampler2D u_refractionMap;
// uniform sampler2D u_depthMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_positionMap;
uniform sampler2D u_dudvMap;
uniform float u_moveFactor;
uniform DirectedLight u_directedLight;
uniform vec3 u_viewPos;
uniform float u_threshold;

uniform int u_fog;
uniform vec3 u_fogColor;
uniform float u_density;
uniform float u_gradient;

uniform mat4 u_view;
uniform mat4 u_projection;

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
in vec4 v_clipSpace;
in vec3 v_fragPos;
in vec2 v_texCoord;

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
    vec2 refractNdc = (v_clipSpace.xy / v_clipSpace.w) / (2.0 * 1.5) + 0.5;

    vec2 reflectTexCoord = vec2(ndc.x, -ndc.y);
    vec2 refractTexCoord = vec2(refractNdc.x, refractNdc.y);

    // float depth = texture(u_depthMap, refractTexCoord).r;
    float waterDepth = distance(texture(u_positionMap, refractTexCoord).xyz, v_fragPos);

    // depth = gl_FragCoord.z;
    // float waterDistance = 2.0 * c_near * c_far / (c_far + c_near - (2.0 * depth - 1.0) * (c_far - c_near));
    // float waterDepth = floorDistance - waterDistance;

    vec2 texCoordDistorted = texture(u_dudvMap, vec2(v_texCoord.x + u_moveFactor, v_texCoord.y)).rg * 0.1;
    texCoordDistorted = v_texCoord + vec2(texCoordDistorted.x, texCoordDistorted.y + u_moveFactor);
    vec2 totalDistortion =
        (texture(u_dudvMap, texCoordDistorted).rg * 2.0 - 1.0) * c_waveStrength * clamp(waterDepth, 0.0, 2.0) / 3.0;

    vec3 normalMapColor = texture(u_normalMap, texCoordDistorted).rgb;
    vec3 normal = normalize(mat3(u_noramlFix) * normalize(normalMapColor * 2.0 - 1.0) * vec3(1.0, 1.0, 1.0));

    vec3 refr = refract(normalize(v_fragPos - u_viewPos), vec3(0, 1, 0), 0.75) * clamp(waterDepth, 0, 2.0) / 3.0;
    vec4 refrFragPos = u_projection * u_view * vec4(v_fragPos + refr, 1.0);
    refractTexCoord = (refrFragPos.xy / refrFragPos.w) / (2.0 * 1.5) + 0.5;

    // vec2 distortion1 =
    //     (texture(u_dudvMap, vec2(v_texCoord.x + u_moveFactor, v_texCoord.y)).rg * 2.0 - 1.0) * c_waveStrength;
    // vec2 distortion2 =
    //     (texture(u_dudvMap, vec2(-v_texCoord.x + u_moveFactor, v_texCoord.y + u_moveFactor)).rg * 2.0 - 1.0) *
    //     c_waveStrength;

    // vec2 totalDistortion = distortion1 + distortion2;

    reflectTexCoord += totalDistortion;
    reflectTexCoord.x = clamp(reflectTexCoord.x, 0.001, 0.999);
    reflectTexCoord.y = clamp(reflectTexCoord.y, -0.999, -0.001);

    refractTexCoord += totalDistortion;
    refractTexCoord.x = clamp(refractTexCoord.x, 0.001, 0.999);
    refractTexCoord.y = clamp(refractTexCoord.y, 0.001, 0.999);

    // vec3 reflectDir = normalize(v_fragPos - texture(u_positionMap, reflectTexCoord).rgb);

    vec3 viewDir = normalize(u_viewPos - v_fragPos);
    float refractFactor = dot(normalize(viewDir), normal);
    refractFactor = clamp(pow(refractFactor, 1), 0.0, 1.0); // * clamp(waterDepth, 0.0, 1.0);

    vec4 refractColor = texture(u_refractionMap, refractTexCoord);
    vec4 reflectColor = texture(u_reflectionMap, reflectTexCoord);

    // refractColor = c_waterColor + (refractColor - c_waterColor) * exp(-waterDepth * 0.1);

    vec3 ambient = u_directedLight.ambient;

    vec3 lightDir = normalize(-u_directedLight.direction);
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = u_directedLight.diffuse * diffuseFactor;

    vec3 halfwayDir = normalize(viewDir - u_directedLight.direction);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), 128);
    vec3 specular = u_directedLight.specular * specularFactor;

    o_fragColor = mix(reflectColor, refractColor, refractFactor);
    o_fragColor = mix(o_fragColor, c_waterColor, 0.2);
    o_fragColor = vec4((ambient + diffuse + specular), 1.0) * o_fragColor;

    // o_fragColor.a = clamp(waterDepth * 2.0, 0.0, 1.0);
    // if (u_fog > 0) {
    //     o_fragColor = mix(vec4(u_fogColor, 1.0), o_fragColor, v_visibility);
    // }
    if (u_fog > 0) {
        vec4 fragCameraPos = u_view * vec4(v_fragPos, 1.0);
        float distanceToCamera = length(fragCameraPos.xyz);
        float fragVisibility = exp(-pow(distanceToCamera * u_density, u_gradient));
        fragVisibility = clamp(fragVisibility, 0.0, 1.0);
        o_fragColor = mix(vec4(u_fogColor, 1.0), o_fragColor, fragVisibility);
    }

    float brightness = dot(o_fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > u_threshold)
        o_brightColor = vec4(o_fragColor.rgb, 1.0);
    else
        o_brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
