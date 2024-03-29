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

vec3 directedLightCalculation(DirectedLight light, FragmentMaterial material, vec3 fragPos, vec3 viewDir);
float directedLightShadowCalculation(DirectedLight light, vec3 fragPos, float bias);

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform int u_hasMaterial;
uniform int u_hasDirectedLight;
uniform DirectedLight u_directedLight;
uniform Material u_material;
uniform vec3 u_viewPos;
uniform float u_threshold;
uniform int u_hasSSAO;
uniform sampler2D u_ssao;
uniform int u_hasNormalMapping;

uniform int u_fog;
uniform vec3 u_fogColor;
uniform float u_density;
uniform float u_gradient;

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
flat in int v_id;
in vec3 v_color;
in vec3 v_normal;
in vec2 v_texCoord;
in vec3 v_fragPos;
in vec3 v_fragCameraPos;
in vec2 v_fragScreenPos;
in mat3 v_TBN;

/////////////////////////////////////////////////////////////
/////////////////////////// OUT /////////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) out vec4 o_fragColor;
layout(location = 1) out int o_id;
layout(location = 2) out vec4 o_brightColor;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    o_id = v_id;

    vec3 viewDir = normalize(u_viewPos - v_fragPos);

    FragmentMaterial material = FragmentMaterial(v_color, v_color, v_normal, 128.0);
    if (u_hasMaterial > 0) {
        vec3 diffuse = v_color;
        if (u_material.hasDiffuse > 0) {
            diffuse = texture(u_material.diffuse, v_texCoord).rgb;
        }

        vec3 specular = v_color;
        if (u_material.hasSpecular > 0) {
            specular = texture(u_material.specular, v_texCoord).rgb;
        }

        vec3 normal = v_normal;
        if (u_hasNormalMapping > 0 && u_material.hasNormal > 0) {
            normal = texture(u_material.normal, v_texCoord).rgb;
            normal = normalize(normal * 2.0 - 1.0);
            normal = normalize(v_TBN * normal);
        }

        material = FragmentMaterial(diffuse, specular, normal, u_material.shininess);
    }

    vec3 result = vec3(0);

    if (u_hasDirectedLight > 0) {
        result += directedLightCalculation(u_directedLight, material, v_fragPos, viewDir);
    }

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
