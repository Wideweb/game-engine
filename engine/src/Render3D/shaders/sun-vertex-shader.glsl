#version 330 core
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTextureCoord;

out vec2 TexCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
    TexCoord = VertexTextureCoord;
    vec4 pos = Projection * View * Model * vec4(VertexPosition, 1.0);
    gl_Position = pos.xyww;
}