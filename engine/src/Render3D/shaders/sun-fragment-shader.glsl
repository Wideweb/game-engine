#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D colorMap;

void main() {
    vec4 color = texture(colorMap, TexCoord);
    color.a = color.a * 0.8;
    FragColor = color;
}
