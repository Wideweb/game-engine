#version 330 core

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
struct DirectedLight {
    int pcf;
    float biasFactor;
    float biasMin;

    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    mat4 spaceMatrix;
    sampler2D shadowMap;
};

struct SpotLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    mat4 spaceMatrix;
    samplerCube shadowMap;
    float farPlane;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

struct FragmentMaterial {
    vec3 diffuse;
    vec3 specular;
    vec3 normal;
    float shininess;
};

vec3 directedLightCalculation(DirectedLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir);
float directedLightShadowCalculation(DirectedLight light, vec3 fragPos, float bias);

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const vec3 c_fogColor = vec3(0.55, 0.69, 0.73);

const vec3 c_sampleOffsetDirections[20] = vec3[](
    vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1),
    vec3(-1, 1, -1), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, 0, 1), vec3(-1, 0, 1),
    vec3(1, 0, -1), vec3(-1, 0, -1), vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1));

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform int u_hasDirectedLight;
uniform DirectedLight u_directedLight;
uniform vec3 u_viewPos;
uniform float u_threshold;

uniform sampler2D u_colorMap;
uniform sampler2D u_positionMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_depthMap;

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
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
    vec3 fragColor = texture(u_colorMap, v_texCoord).rgb;
    vec3 fragPos = texture(u_positionMap, v_texCoord).rgb;
    vec3 fragNormal = texture(u_normalMap, v_texCoord).rgb;
    vec3 fragSpecular = texture(u_specularMap, v_texCoord).rgb;
    float fragShininess = texture(u_specularMap, v_texCoord).a;
    vec3 fragDepth = texture(u_depthMap, v_texCoord).rgb;
    float fragVisibility = texture(u_colorMap, v_texCoord).a;

    vec3 viewDir = normalize(u_viewPos - fragPos);

    FragmentMaterial material = FragmentMaterial(fragColor, fragSpecular, fragNormal, fragShininess);

    vec3 result = vec3(0);

    if (u_hasDirectedLight > 0) {
        result += directedLightCalculation(u_directedLight, material, fragPos, viewDir);
    }

    o_fragColor = vec4(result, 1.0);
    o_fragColor = mix(vec4(c_fogColor, 1.0), o_fragColor, fragVisibility);

    float brightness = dot(o_fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > u_threshold)
        o_brightColor = vec4(o_fragColor.rgb, 1.0);
    else
        o_brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

/////////////////////////////////////////////////////////////
////////////////////// DIRECTED LIGHT ///////////////////////
/////////////////////////////////////////////////////////////
vec3 directedLightCalculation(DirectedLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir) {
    vec3 ambient = light.ambient;

    vec3 lightDir = normalize(-light.direction);
    float diffuseFactor = max(dot(material.normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseFactor;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(material.normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * material.specular * specularFactor;

    float bias = max(light.biasFactor * (1.0 - dot(material.normal, lightDir)), light.biasMin);
    float shadow = directedLightShadowCalculation(light, fragPos, bias);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * material.diffuse;

    return lighting;
}

float directedLightShadowCalculation(DirectedLight light, vec3 fragPos, float bias) {
    vec4 fragPosInLightSpace = light.spaceMatrix * vec4(fragPos, 1.0);
    vec3 coords = fragPosInLightSpace.xyz / fragPosInLightSpace.w;
    coords = coords * 0.5 + 0.5;
    float currentDepth = coords.z;

    // float closestDepth = texture(light.shadowMap, coords.xy).r;
    // float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    if (currentDepth > 1.0) {
        return 0.0;
    }

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(light.shadowMap, 0);
    for (int x = -light.pcf; x <= light.pcf; ++x) {
        for (int y = -light.pcf; y <= light.pcf; ++y) {
            float pcfDepth = texture(light.shadowMap, coords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= (light.pcf * 2 + 1) * (light.pcf * 2 + 1);

    return shadow;
}