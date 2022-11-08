/////////////////////////////////////////////////////////////
///////////////////// PBR LIGHT BEGIN ///////////////////////
/////////////////////////////////////////////////////////////

#include "lib/light/directed-light-data.glsl"
#include "lib/light/spot-light-data.glsl"
#include "lib/light/pbr-material-data.glsl"
#include "lib/light/directed-light-shadow.glsl"
#include "lib/light/spot-light-shadow.glsl"

#ifdef SSAO
#include "lib/ssao/fragment.glsl"
#endif

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
vec3 directedLightEnergyBrightness(DirectedLight light, FragmentPBRMaterial material, vec3 fragPos, vec3 viewDir,
                                   vec3 surfaceReflectance);

vec3 spotLightEnergyBrightness(SpotLight light, FragmentPBRMaterial material, vec3 fragPos, vec3 viewDir,
                               vec3 surfaceReflectance);

vec3 lightEnergyBrightness(vec3 lightDir, vec3 lightColor, FragmentPBRMaterial material, vec3 fragPos, vec3 viewDir,
                           vec3 surfaceReflectance);

float distributionGGX(vec3 N, vec3 H, float roughness);

float geometrySchlickGGX(float NdotV, float roughness);

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

vec3 fresnelSchlick(float cosTheta, vec3 F0);

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const float PI = 3.14159265359;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
vec4 pbr(FragmentMaterial material, vec3 viewPos, vec3 fragPos) {
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 surfaceReflectance = vec3(0.04);
    surfaceReflectance = mix(surfaceReflectance, material.albedo, material.metallic);

    vec3 Lo = vec3(0.0);

    for (int i = 0; i < u_directedLightsNumber; i++) {
        Lo += directedLightEnergyBrightness(u_directedLights[i], material, fragPos, viewDir, surfaceReflectance);
    }

    for (int i = 0; i < u_spotLightsNumber; i++) {
        Lo += spotLightEnergyBrightness(u_spotLights[i], material, fragPos, viewDir, surfaceReflectance);
    }

    vec3 ambient = vec3(0.03) * material.albedo * material.ambientOcclusion;

#ifdef SSAO
    ambient *= ssao();
#endif

    return vec4(ambient + Lo, 1.0);
}

/////////////////////////////////////////////////////////////
////////////////////// DIRECTED LIGHT ///////////////////////
/////////////////////////////////////////////////////////////
vec3 directedLightEnergyBrightness(DirectedLight light, FragmentPBRMaterial material, vec3 fragPos, vec3 viewDir,
                                   vec3 surfaceReflectance) {
    vec3 lightDir = normalize(-light.direction);
    float bias = max(light.biasFactor * (1.0 - dot(material.normal, lightDir)), light.biasMin);
    float shadow = 0.0;

#ifdef DIRECTED_LIGHT_SHADOW
    shadow = directedLightShadowCalculation(light, fragPos, bias);
#endif

    vec3 LE = lightEnergyBrightness(lightDir, light.diffuse, material, fragPos, viewDir, surfaceReflectance);
    return (1.0 - shadow) * LE;
}

/////////////////////////////////////////////////////////////
/////////////////////// SPOT LIGHT //////////////////////////
/////////////////////////////////////////////////////////////
vec3 spotLightEnergyBrightness(SpotLight light, FragmentPBRMaterial material, vec3 fragPos, vec3 viewDir,
                               vec3 surfaceReflectance) {
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float shadow = 0.0;

#ifdef SPOT_LIGHT_SHADOW
    shadow = spotLightShadowCalculation(light, fragPos, light.bias);
#endif

    vec3 LE = lightEnergyBrightness(lightDir, light.diffuse, material, fragPos, viewDir, surfaceReflectance);
    return (1.0 - shadow) * LE * attenuation;
}

vec3 LightEnergyBrightness(vec3 lightDir, vec3 lightColor, FragmentPBRMaterial material, vec3 fragPos, vec3 viewDir,
                           vec3 surfaceReflectance) {
    vec3 halfwayDir = normalize(viewDir + lightDir);
    vec3 radiance = lightColor;

    // Cook-Torrance BRDF
    float NDF = distributionGGX(material.normal, halfwayDir, material.roughness);
    float G = geometrySmith(material.normal, viewDir, lightDir, material.roughness);
    vec3 F = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), surfaceReflectance);

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
vec3 fresnelSchlick(float cosTheta, vec3 F0) { return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0); }

// Функция нормального распределения.
// Аппроксимация общего выравнивания микрограней по медианному вектору H
// с учетом параметра шероховатости
float distributionGGX(vec3 N, vec3 H, float roughness) {
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
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

/////////////////////////////////////////////////////////////
////////////////////// PBR LIGHT END ////////////////////////
/////////////////////////////////////////////////////////////
