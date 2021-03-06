#version 330 core

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTextureCoord;
layout(location = 3) in mat4 Model;

uniform mat4 View;
uniform mat4 Projection;
// uniform mat4 LightSpaceMatrix;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
// out vec4 FragPosLightSpace;

void main() {
    gl_Position = Projection * View * Model * vec4(VertexPosition, 1.0);
    TexCoord = vec2(VertexTextureCoord.x, 1.0 - VertexTextureCoord.y);
    FragPos = vec3(Model * vec4(VertexPosition, 1.0));
    Normal = mat3(transpose(inverse(Model))) * VertexNormal;
    // FragPosLightSpace = LightSpaceMatrix * vec4(FragPos, 1.0);
}