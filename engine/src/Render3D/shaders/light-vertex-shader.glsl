#version 330 core

layout(location = 0) in vec3 VertexPosition;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 Color;

void main() {
    gl_Position = Projection * View * Model * vec4(VertexPosition, 1.0f);
    Color = vec3(1.0, 1.0, 1.0);
}