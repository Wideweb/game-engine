#version 330 core
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec3 gColor;
layout(location = 3) out vec4 gSpecular;

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

void main() {
    gPosition = FragPos;
    gNormal = normalize(Normal);
    gColor = texture(Material.diffuse, TexCoord).rgb;
    gSpecular.rgb = texture(Material.specular, TexCoord).rgb;
    gSpecular.a = Material.shininess;
}