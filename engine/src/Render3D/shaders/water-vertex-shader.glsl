#version 330 core
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec2 VertexTextureCoord;

out vec4 ClipSpace;
out vec2 TexCoord;
out vec3 ViewDir;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;
uniform vec3 ViewPos;

void main() {
    vec4 worldPos = Model * vec4(VertexPosition, 1.0);
    ClipSpace = Projection * View * worldPos;
    TexCoord = VertexTextureCoord;
    ViewDir = ViewPos - worldPos.xyz;
    gl_Position = ClipSpace;
}