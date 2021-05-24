#version 330 core

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
struct DirectedLight {
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

vec3 spotLightCalculation(SpotLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir);
float spotLightShadowCalculation(SpotLight light, vec3 fragPos, float bias);

vec3 directedLightCalculation(DirectedLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir);
float directedLightShadowCalculation(DirectedLight light, vec3 fragPos, float bias);

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const int c_maxSpotLights = 4;
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
uniform SpotLight u_spotLights[c_maxSpotLights];
uniform int u_spotLightsNumber;
uniform Material u_material;
uniform vec3 u_viewPos;

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
in vec2 v_texCoord;
in vec3 v_fragPos;
in vec3 v_normal;
in float v_visibility;

/////////////////////////////////////////////////////////////
/////////////////////////// OUT /////////////////////////////
/////////////////////////////////////////////////////////////
out vec4 out_fragColor;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    vec3 viewDir = normalize(u_viewPos - v_fragPos);

    vec3 diffuse = texture(u_material.diffuse, v_texCoord).rgb;
    vec3 specular = texture(u_material.specular, v_texCoord).rgb;

    FragmentMaterial material = FragmentMaterial(diffuse, specular, v_normal, u_material.shininess);

    vec3 result = vec3(0);

    if (u_hasDirectedLight > 0) {
        result += directedLightCalculation(u_directedLight, material, v_fragPos, viewDir);
    }

    // if (u_spotLightsNumber > 0) {
    //     result += spotLightCalculation(u_spotLights[0], material, v_fragPos, viewDir);
    // }

    out_fragColor = vec4(result, 1.0);
    out_fragColor = mix(vec4(c_fogColor, 1.0), out_fragColor, v_visibility);
}

/////////////////////////////////////////////////////////////
/////////////////////// SPOT LIGHT //////////////////////////
/////////////////////////////////////////////////////////////
vec3 spotLightCalculation(SpotLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir) {
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * material.diffuse;

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(material.normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    vec3 reflectDir = reflect(-lightDir, material.normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(material.normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float bias = max(0.05 * (1.0 - dot(material.normal, lightDir)), 0.005);
    float shadow = spotLightShadowCalculation(light, fragPos, bias);
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);

    return lighting;
}

float spotLightShadowCalculation(SpotLight light, vec3 fragPos, float bias) {
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    int samples = 20;

    float diskRadius = 0.02;
    for (int i = 0; i < samples; ++i) {
        float closestDepth = texture(light.shadowMap, fragToLight + c_sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= light.farPlane;
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

/////////////////////////////////////////////////////////////
////////////////////// DIRECTED LIGHT ///////////////////////
/////////////////////////////////////////////////////////////
vec3 directedLightCalculation(DirectedLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir) {
    vec3 ambient = light.ambient;

    vec3 lightDir = normalize(-light.direction);
    float diffuseFactor = max(dot(material.normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseFactor;

    vec3 reflectDir = reflect(-lightDir, material.normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(material.normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * material.specular * specularFactor * 0.002;

    float bias = max(0.05 * (1.0 - dot(material.normal, lightDir)), 0.005);
    float shadow = directedLightShadowCalculation(light, fragPos, bias);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * material.diffuse;

    return lighting;
}

float directedLightShadowCalculation(DirectedLight light, vec3 fragPos, float bias) {
    vec4 fragPosInLightSpace = light.spaceMatrix * vec4(fragPos, 1.0);
    vec3 coords = fragPosInLightSpace.xyz / fragPosInLightSpace.w;
    coords = coords * 0.5 + 0.5;
    float closestDepth = texture(light.shadowMap, coords.xy).r;
    float currentDepth = coords.z;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}
