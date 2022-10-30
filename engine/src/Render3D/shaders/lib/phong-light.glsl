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

    samplerCube shadowMap;
    float farPlane;
    float bias;
    float pcfDiskRadius;
    int pcfSamples;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;

    int hasDiffuse;
    int hasSpecular;
    int hasNormal;
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
const vec3 c_sampleOffsetDirections[20] = vec3[](
    vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1),
    vec3(-1, 1, -1), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, 0, 1), vec3(-1, 0, 1),
    vec3(1, 0, -1), vec3(-1, 0, -1), vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1));

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

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(material.normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float shadow = spotLightShadowCalculation(light, fragPos, light.bias);
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);

    return lighting;
}

float spotLightShadowCalculation(SpotLight light, vec3 fragPos, float bias) {
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;

    for (int i = 0; i < light.pcfSamples; ++i) {
        float closestDepth =
            texture(light.shadowMap, fragToLight + c_sampleOffsetDirections[i] * light.pcfDiskRadius).r;
        closestDepth *= light.farPlane;
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(light.pcfSamples);

    return shadow;
}

/////////////////////////////////////////////////////////////
////////////////////// DIRECTED LIGHT ///////////////////////
/////////////////////////////////////////////////////////////
vec3 directedLightCalculation(DirectedLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir) {
    float ambientOcclusion = 1.0;
    if (u_hasSSAO > 0) {
        ambientOcclusion = texture(u_ssao, v_fragScreenPos * 0.5 + 0.5).r;
    }

    vec3 ambient = light.ambient * ambientOcclusion;

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