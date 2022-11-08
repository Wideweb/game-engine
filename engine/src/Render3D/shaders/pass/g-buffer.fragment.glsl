#version 330 core

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
#define PHONG
#define NOMRAL_MAPPING

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform vec4 u_clipPlane;

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
in vec3 v_color;
in vec2 v_texCoord;
in vec4 v_fragPos;
in vec4 v_currPos;
in vec4 v_prevPos;

/////////////////////////////////////////////////////////////
//////////////////////////// OUT ////////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) out vec4 o_gColor;
layout(location = 1) out vec3 o_gPosition;
layout(location = 2) out vec3 o_gMotion;
layout(location = 3) out vec3 o_gNormal;

#ifdef PHONG
layout(location = 4) out vec4 o_gSpecular;
#endif

#ifdef PBR
layout(location = 4) out vec3 o_gMetallic_Roughness_AmbientOcclusion;
#endif

#ifdef PHONG
#include "lib/light/phong-material-data.glsl"
#include "lib/normal/fragment.glsl"
#endif

#ifdef PBR
#include "lib/light/pbr-material-data.glsl"
#include "lib/normal/fragment.glsl"
#endif

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////

void main() {
    if (dot(v_fragPos, u_clipPlane) < 0) {
        discard;
    }

    // vec3 diffuse = v_color;
    // vec3 specular = v_color;
    // vec3 normal = v_normal;
    // float shininess = 128.0;

    FragmentMaterial material = getFragmentMaterial(v_texCoord, getNormal());

    o_gPosition = v_fragPos.xyz;
    o_gNormal = material.normal;

    vec2 currScreenPos = v_currPos.xy / v_currPos.w * 0.5 + vec2(0.5);
	vec2 prevScreenPos = v_prevPos.xy / v_prevPos.w * 0.5 + vec2(0.5);

	o_gMotion = vec3(currScreenPos - prevScreenPos, 0.0);

#ifdef PHONG
    o_gColor = vec4(material.diffuse, 1.0);
    o_gSpecular = vec4(material.specular, material.shininess);
#endif

#ifdef PBR
    o_gColor = vec4(material.albedo, 1.0);
    o_gMetallic_Roughness_AmbientOcclusion = vec3(material.metallic, material.roughness, material.ambientOcclusion);
#endif
}