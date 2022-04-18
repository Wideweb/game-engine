#version 330 core

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    float shininess;

    int hasDiffuse;
    int hasSpecular;
    int hasNormal;
};

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform int u_hasMaterial;
uniform Material u_material;
uniform vec4 u_clipPlane;
uniform int u_hasNormalMapping;

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
in vec3 v_color;
in vec3 v_normal;
in vec2 v_texCoord;
in vec4 v_fragPos;
in mat3 v_TBN;

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

    vec3 diffuse = v_color;
    vec3 specular = v_color;
    vec3 normal = v_normal;
    float shininess = 128.0;

    if (u_hasMaterial > 0) {
        if (u_material.hasDiffuse > 0) {
            diffuse = texture(u_material.diffuse, v_texCoord).rgb;
        }

        if (u_material.hasSpecular > 0) {
            specular = texture(u_material.specular, v_texCoord).rgb;
        }

        if (u_hasNormalMapping > 0 && u_material.hasNormal > 0) {
            normal = texture(u_material.normal, v_texCoord).rgb;
            normal = normalize(normal * 2.0 - 1.0);
            normal = normalize(v_TBN * normal);
        }

        shininess = u_material.shininess;
    }

    o_gColor = vec4(diffuse, 1.0);
    o_gPosition = v_fragPos.xyz;
    o_gNormal = normal;
    o_gSpecular.rgb = specular;
    o_gSpecular.a = shininess;
}