#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D colorMap;
uniform float alpha;

void main() {
    FragColor = vec4(texture(colorMap, TexCoord).rrr, 1.0);
    // FragColor.a = FragColor.a * alpha;
}
