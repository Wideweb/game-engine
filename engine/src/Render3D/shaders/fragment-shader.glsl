#version 330 core
out vec4 FragColor;

struct TLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    samplerCube depthMap;
    float farPlane;
};

struct TMaterial {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
// in vec4 FragPosLightSpace;

uniform TMaterial Material;
uniform vec3 ViewPos;

#define NR_LIGHTS 4
uniform TLight Lights[NR_LIGHTS];
uniform int LightsNumber;

// uniform sampler2D shadowMap;

// float shadowCalculation(vec4 fragPosLightSpace, float bias) {
//     vec3 coords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//     coords = coords * 0.5 + 0.5;

//     if (coords.z > 1.0)
//         return 0.0;

//     float closestDepth = texture(shadowMap, coords.xy).r;
//     float currentDepth = coords.z;

//     float shadow = 0.0;
//     vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
//     for (int x = -1; x <= 1; ++x) {
//         for (int y = -1; y <= 1; ++y) {
//             float pcfDepth =
//                 texture(shadowMap, coords.xy + vec2(x, y) * texelSize).r;
//             shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
//         }
//     }
//     shadow /= 9.0;

//     return shadow;
// }

vec3 sampleOffsetDirections[20] =
    vec3[](vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
           vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
           vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
           vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
           vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1));

float shadowCalculation(TLight light, vec3 fragPos) {
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);

    // float closestDepth = texture(depthMap, fragToLight).r;
    // closestDepth *= FarPlane;
    // float bias = 0.05;
    // return currentDepth - bias > closestDepth ? 1.0 : 0.0;

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(ViewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / light.farPlane)) / 20.0;
    for (int i = 0; i < samples; ++i) {
        float closestDepth =
            texture(light.depthMap,
                    fragToLight + sampleOffsetDirections[i] * diskRadius)
                .r;
        closestDepth *= light.farPlane;
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

vec3 lightCalculation(TLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * texture(Material.diffuse, TexCoord).rgb;

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse =
        light.diffuse * diff * texture(Material.diffuse, TexCoord).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), Material.shininess);
    vec3 specular =
        light.specular * spec * texture(Material.specular, TexCoord).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = shadowCalculation(light, fragPos);
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);

    return lighting;
}

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(ViewPos - FragPos);

    vec3 result = vec3(0);
    for (int i = 0; i < LightsNumber; i++)
        result += lightCalculation(Lights[i], normal, FragPos, viewDir) *
                  (1.0 / LightsNumber);

    FragColor = vec4(result, 1.0);
}