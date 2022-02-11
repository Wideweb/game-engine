#version 330 core

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;
};

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const vec3 c_fogColor = vec3(0.55, 0.69, 0.73);

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform int u_hasMaterial;
uniform Material u_material;
uniform vec4 u_clipPlane;

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
in vec3 v_color;
in vec3 v_normal;
in vec2 v_texCoord;
in vec4 v_fragPos;
in mat3 v_TBN;
in float v_visibility;

/////////////////////////////////////////////////////////////
//////////////////////////// OUT ////////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) out vec4 o_gColor;
layout(location = 1) out vec3 o_gPosition;
layout(location = 2) out vec3 o_gNormal;
layout(location = 3) out vec4 o_gSpecular;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    if (dot(v_fragPos, u_clipPlane) < 0) {
        discard;
    }

    vec3 color = v_color;
    vec3 normal = v_normal;
    float shininess = 128.0;

    if (u_hasMaterial > 0) {
        normal = texture(u_material.normal, v_texCoord).rgb;
        normal = normalize(normal * 2.0 - 1.0);
        normal = normalize(v_TBN * normal);

        color = texture(u_material.specular, v_texCoord).rgb;
        shininess = u_material.shininess;
    }

    o_gColor = vec4(v_color, 1.0);
    o_gPosition = v_fragPos.xyz;
    o_gNormal = normal;
    o_gSpecular.rgb = color;
    o_gSpecular.a = shininess;
}