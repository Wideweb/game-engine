/////////////////////////////////////////////////////////////
//////////////////// PHONG LIGHT BEGIN //////////////////////
/////////////////////////////////////////////////////////////

#include "lib/light/directed-light-data.glsl"
#include "lib/light/spot-light-data.glsl"
#include "lib/light/phong-material-data.glsl"
#include "lib/light/directed-light-shadow.glsl"
#include "lib/light/spot-light-shadow.glsl"

#ifdef SSAO
#include "lib/ssao/fragment.glsl"
#endif

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
vec3 spotLightCalculation(SpotLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir);

vec3 directedLightCalculation(DirectedLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir);

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
vec4 phong(FragmentMaterial material, vec3 viewPos, vec3 fragPos) {
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 Lo = vec3(0.0);

    for (int i = 0; i < u_directedLightsNumber && i < c_maxDirectedLightsNumber; i++) {
        Lo += directedLightCalculation(u_directedLights[i], material, fragPos, viewDir);
    }

    for (int i = 0; i < u_spotLightsNumber && i < c_maxSpotLights; i++) {
        Lo += spotLightCalculation(u_spotLights[i], material, fragPos, viewDir);
    }

    return vec4(Lo, 1.0);
}

/////////////////////////////////////////////////////////////
////////////////////// DIRECTED LIGHT ///////////////////////
/////////////////////////////////////////////////////////////
vec3 directedLightCalculation(DirectedLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir) {
    vec3 ambient = light.ambient;

#ifdef SSAO
    ambient *= ssao();
#endif

    vec3 lightDir = normalize(-light.direction);
    float diffuseFactor = max(dot(material.normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseFactor;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(material.normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * material.specular * specularFactor;

    float bias = max(light.biasFactor * (1.0 - dot(material.normal, lightDir)), light.biasMin);
    float shadow = 0.0;

#ifdef DIRECTED_LIGHT_SHADOW
    shadow = directedLightShadowCalculation(light, fragPos, bias);
#endif

    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * material.diffuse;

    return lighting;
}

/////////////////////////////////////////////////////////////
/////////////////////// SPOT LIGHT //////////////////////////
/////////////////////////////////////////////////////////////
vec3 spotLightCalculation(SpotLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir) {
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * material.diffuse;
#ifdef SSAO
    ambient *= ssao();
#endif

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(material.normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(material.normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float shadow = 0.0;

#ifdef SPOT_LIGHT_SHADOW
    shadow = spotLightShadowCalculation(light, fragPos, light.bias);
#endif

    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);

    return lighting;
}

/////////////////////////////////////////////////////////////
///////////////////// PHONG LIGHT END ///////////////////////
/////////////////////////////////////////////////////////////