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
    sampler2D normal;
    sampler2D metallic;
    sampler2D roughness;
    sampler2D ambientOcclusion;

    int hasDiffuse;
    int hasNormal;
    int hasMetallic;
    int hasRoughness;
    int hasAmbientOcclusion;
};

struct FragmentMaterial {
    vec3 albedo;
    vec3 normal;
    float metallic;
    float roughness;
    float ambientOcclusion;
};

float spotLightShadowCalculation(SpotLight light, vec3 fragPos, float bias);

float directedLightShadowCalculation(DirectedLight light, vec3 fragPos, float bias);

vec3 DirectedLightEnergyBrightness(DirectedLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir,
                                   vec3 surfaceReflectance);

vec3 SpotLightEnergyBrightness(SpotLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir,
                               vec3 surfaceReflectance);

vec3 LightEnergyBrightness(vec3 lightDir, vec3 lightColor, FragmentMaterial material, vec3 fragPos, vec3 viewDir,
                           vec3 surfaceReflectance);

float DistributionGGX(vec3 N, vec3 H, float roughness);

float GeometrySchlickGGX(float NdotV, float roughness);

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

vec3 FresnelSchlick(float cosTheta, vec3 F0);

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const float PI = 3.14159265359;

const int c_maxSpotLights = 4;

const vec3 c_sampleOffsetDirections[20] = vec3[](
    vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1),
    vec3(-1, 1, -1), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, 0, 1), vec3(-1, 0, 1),
    vec3(1, 0, -1), vec3(-1, 0, -1), vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1));

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void PBR(FragmentMaterial material) {
    vec3 viewDir = normalize(u_viewPos - v_fragPos);

    FragmentMaterial material = FragmentMaterial(v_color, v_normal, 0.0, 0.0, 1.0);
    if (u_hasMaterial > 0) {
        vec3 albedo = v_color;
        if (u_material.hasDiffuse > 0) {
            albedo = pow(texture(u_material.diffuse, v_texCoord).rgb, vec3(2.2));
        }

        vec3 normal = v_normal;
        if (u_hasNormalMapping > 0 && u_material.hasNormal > 0) {
            normal = texture(u_material.normal, v_texCoord).rgb;
            normal = normalize(normal * 2.0 - 1.0);
            normal = normalize(v_TBN * normal);
        }

        float metallic = 0;
        if (u_material.hasMetallic > 0) {
            metallic = texture(u_material.metallic, v_texCoord).r;
        }

        float roughness = 0;
        if (u_material.hasRoughness > 0) {
            roughness = texture(u_material.roughness, v_texCoord).r;
        }

        float ambientOcclusion = 0;
        if (u_material.hasAmbientOcclusion > 0) {
            ambientOcclusion = texture(u_material.ambientOcclusion, v_texCoord).r;
        }

        material = FragmentMaterial(albedo, normal, metallic, roughness, ambientOcclusion);
    }

    vec3 result = vec3(0);

    vec3 surfaceReflectance = vec3(0.04);
    surfaceReflectance = mix(surfaceReflectance, material.albedo, material.metallic);

    vec3 Lo = vec3(0.0);

    if (u_hasDirectedLight > 0) {
        Lo += DirectedLightEnergyBrightness(u_directedLight, material, v_fragPos, viewDir, surfaceReflectance);
    }

    if (u_spotLightsNumber > 0) {
        Lo += SpotLightEnergyBrightness(u_spotLights[0], material, v_fragPos, viewDir, surfaceReflectance);
    }

    vec3 ambient = vec3(0.03) * material.albedo * material.ambientOcclusion;
    if (u_hasSSAO > 0) {
        ambient *= texture(u_ssao, v_fragScreenPos * 0.5 + 0.5).r;
    }
    result = ambient + Lo;

    o_fragColor = vec4(result, 1.0);
    if (u_fog > 0) {
        float distanceToCamera = length(v_fragCameraPos.xyz);
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

/////////////////////////////////////////////////////////////
////////////////////// DIRECTED LIGHT ///////////////////////
/////////////////////////////////////////////////////////////
vec3 DirectedLightEnergyBrightness(DirectedLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir,
                                   vec3 surfaceReflectance) {
    vec3 lightDir = normalize(-light.direction);
    float bias = max(light.biasFactor * (1.0 - dot(material.normal, lightDir)), light.biasMin);
    float shadow = directedLightShadowCalculation(light, fragPos, bias);
    vec3 LE = LightEnergyBrightness(lightDir, light.diffuse, material, fragPos, viewDir, surfaceReflectance);
    return (1.0 - shadow) * LE;
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

/////////////////////////////////////////////////////////////
/////////////////////// SPOT LIGHT //////////////////////////
/////////////////////////////////////////////////////////////
vec3 SpotLightEnergyBrightness(SpotLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir,
                               vec3 surfaceReflectance) {
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float shadow = spotLightShadowCalculation(light, v_fragPos, light.bias);
    vec3 LE = LightEnergyBrightness(lightDir, light.diffuse, material, fragPos, viewDir, surfaceReflectance);
    return (1.0 - shadow) * LE * attenuation;
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
/////////////////////////// PBR /////////////////////////////
/////////////////////////////////////////////////////////////
vec3 LightEnergyBrightness(vec3 lightDir, vec3 lightColor, FragmentMaterial material, vec3 fragPos, vec3 viewDir,
                           vec3 surfaceReflectance) {
    vec3 halfwayDir = normalize(viewDir + lightDir);
    vec3 radiance = lightColor;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(material.normal, halfwayDir, material.roughness);
    float G = GeometrySmith(material.normal, viewDir, lightDir, material.roughness);
    vec3 F = FresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), surfaceReflectance);

    vec3 kSpecular = F;
    vec3 kDiffuse = vec3(1.0) - kSpecular;
    kDiffuse *= 1.0 - material.metallic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(material.normal, viewDir), 0.0) * max(dot(material.normal, lightDir), 0.0);
    vec3 specular = numerator / max(denominator, 0.001);

    float normalDotLightDir = max(dot(material.normal, lightDir), 0.0);
    vec3 Lo = (kDiffuse * material.albedo / PI + specular) * radiance * normalDotLightDir;

    return Lo;
}

// Уравнение Френеля
// Описывает отношение отраженного и преломленного света,
// которое зависит от угла, под который мы смотрим на поверхность
vec3 FresnelSchlick(float cosTheta, vec3 F0) { return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0); }

// Функция нормального распределения.
// Аппроксимация общего выравнивания микрограней по медианному вектору H
// с учетом параметра шероховатости
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

// Функция геометрии
// Статистически аппроксимирует относительную площадь поверхности,
// где ее микроскопические неровности перекрывают друг друга
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}