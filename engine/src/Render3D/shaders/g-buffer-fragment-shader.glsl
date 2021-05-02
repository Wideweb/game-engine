#version 330 core
layout(location = 0) out vec4 gColor;
layout(location = 1) out vec3 gPosition;
layout(location = 2) out vec3 gNormal;
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
in float Visibility;

uniform TMaterial Material;
uniform float ClipY;

const vec3 fogColor = vec3(0.55, 0.69, 0.73);

void main() {
    if (FragPos.y < ClipY) {
        discard;
    }

    gPosition = FragPos;
    gNormal = normalize(Normal);
    gSpecular.rgb = texture(Material.specular, TexCoord).rgb;
    gSpecular.a = Material.shininess;

    gColor = texture(Material.diffuse, TexCoord);
    // gColor = mix(vec4(fogColor, 1.0), gColor, Visibility);
}