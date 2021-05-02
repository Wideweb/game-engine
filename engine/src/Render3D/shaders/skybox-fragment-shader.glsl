#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube skybox;

const float lowerLimit = 0.0;
const float upperLimit = 0.3;

const vec3 fogColor = vec3(0.55, 0.69, 0.73);

void main() {
    vec4 color = texture(skybox, TexCoord);

    float factor = (TexCoord.y - lowerLimit) / (upperLimit - lowerLimit);
    factor = clamp(factor, 0.0, 1.0);

    FragColor = mix(vec4(fogColor, 1.0), color, factor);
}
