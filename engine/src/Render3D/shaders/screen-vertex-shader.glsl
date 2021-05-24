#version 330 core
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTextureCoord;

out vec2 TexCoord;

uniform mat4 Model;

void main() {
    TexCoord = VertexTextureCoord;
    gl_Position = Model * vec4(VertexPosition, 1.0);
}