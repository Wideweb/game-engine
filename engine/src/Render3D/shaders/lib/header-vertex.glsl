/////////////////////////////////////////////////////////////
//////////////////////// ATTRIBUTES /////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) in int a_id;
layout(location = 1) in mat4 a_model;
layout(location = 5) in vec3 a_vertexPosition;
layout(location = 6) in vec3 a_vertexNormal;
layout(location = 7) in vec2 a_vertexTextureCoord;
layout(location = 8) in vec3 a_vertexTangent;
layout(location = 9) in vec3 a_vertexBitangent;
layout(location = 10) in vec3 a_vertexColor;

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform mat4 u_view;
uniform mat4 u_projection;

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
flat out int v_id;
out vec3 v_color;
out vec2 v_texCoord;
out vec3 v_fragPos;
out vec3 v_fragCameraPos;

#include "lib/normal/vertex.glsl"
#include "lib/skelet/vertex.glsl"

#ifdef SSAO
#include "lib/ssao/vertex.glsl"
#endif

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void super() {
    v_id = a_id;

    mat4 modelMatrix = a_model * getSkeletVertexTransform();
    vec4 worldPosition = modelMatrix * vec4(a_vertexPosition, 1.0);
    v_color = a_vertexColor;
    v_texCoord = vec2(a_vertexTextureCoord.x, a_vertexTextureCoord.y);
    v_fragPos = vec3(worldPosition);
    v_fragCameraPos = vec3(u_view * worldPosition);

    handleNormal(modelMatrix, a_vertexTangent, a_vertexNormal);

#ifdef SSAO
    ssao(u_projection, v_fragCameraPos);
#endif

    gl_Position = u_projection * vec4(v_fragCameraPos, 1.0);;
}