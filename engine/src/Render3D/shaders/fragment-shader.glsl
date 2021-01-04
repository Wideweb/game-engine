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

uniform TMaterial Material;
uniform TLight Light;
uniform vec3 ViewPos;

void main() {
    float distance = length(Light.position - FragPos);
    float attenuation = 1.0 / (Light.constant + Light.linear * distance +
                               Light.quadratic * (distance * distance));

    vec3 ambient = Light.ambient * texture(Material.diffuse, TexCoord).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(Light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =
        Light.diffuse * diff * texture(Material.diffuse, TexCoord).rgb;

    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), Material.shininess);
    vec3 specular =
        Light.specular * spec * texture(Material.specular, TexCoord).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    FragColor = vec4(ambient + diffuse, 1.0f);
}
