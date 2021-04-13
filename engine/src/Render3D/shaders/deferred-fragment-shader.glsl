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
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec2 TexCoord;

uniform vec3 ViewPos;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform sampler2D gSpecular;
uniform sampler2D gDepth;

#define NR_LIGHTS 4
uniform TLight Lights[NR_LIGHTS];
uniform int LightsNumber;

vec3 sampleOffsetDirections[20] =
    vec3[](vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
           vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
           vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
           vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
           vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1));

float shadowCalculation(TLight light, vec3 fragPos) {
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    // float viewDistance = length(ViewPos - fragPos);
    // float diskRadius = (1.0 + (viewDistance / light.farPlane)) / 20.0;
    float diskRadius = 0.02;
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

vec3 lightCalculation(TLight light, TMaterial material, vec3 normal,
                      vec3 fragPos, vec3 viewDir) {
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * material.diffuse;

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = shadowCalculation(light, fragPos);
    vec3 lighting = ambient + (1.0 - shadow) * (diffuse + specular);

    return lighting;
}

void main() {
    vec3 fragPos = texture(gPosition, TexCoord).rgb;
    vec3 normal = texture(gNormal, TexCoord).rgb;
    vec3 diffuse = texture(gColor, TexCoord).rgb;
    vec3 specular = texture(gSpecular, TexCoord).rgb;
    float shininess = texture(gSpecular, TexCoord).a;
    vec3 viewDir = normalize(ViewPos - fragPos);

    TMaterial material = TMaterial(diffuse, specular, shininess);

    vec3 result = vec3(0);
    for (int i = 0; i < LightsNumber; i++)
        result +=
            lightCalculation(Lights[i], material, normal, fragPos, viewDir) *
            (1.0 / LightsNumber);

    FragColor = vec4(result, 1.0);
    gl_FragDepth = texture(gDepth, TexCoord).r;
}