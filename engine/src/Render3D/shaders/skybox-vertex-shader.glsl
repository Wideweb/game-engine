#version 330 core
layout(location = 0) in vec3 VertexPosition;

out vec3 TexCoord;

uniform mat4 View;
uniform mat4 Projection;

void main() {
    TexCoord = VertexPosition;
    vec4 pos = Projection * View * vec4(VertexPosition, 1.0);
    gl_Position = pos.xyww;
}