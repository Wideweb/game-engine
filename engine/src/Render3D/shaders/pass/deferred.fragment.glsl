#version 330 core

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
#define DEFERRED
#define PHONG
#define NOMRAL_MAPPING

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform mat4 u_view;
uniform vec3 u_viewPos;

uniform sampler2D u_colorMap;
uniform sampler2D u_positionMap;
uniform sampler2D u_normalMap;

#ifdef PHONG
uniform sampler2D u_specularMap;
#endif

#ifdef PBR
uniform sampler2D u_metallic_roughness_ambientOcclusion_map;
#endif

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
in vec2 v_texCoord;

/////////////////////////////////////////////////////////////
/////////////////////////// OUT /////////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) out vec4 o_fragColor;

#ifdef FOG
#include "lib/fog/fragment.glsl"
#endif

#ifdef BRIGHTNESS
#include "lib/brightness/fragment.glsl"
#endif

#ifdef PHONG
#include "lib/light/phong.glsl"
#endif

#ifdef PBR
#include "lib/light/pbr.glsl"
#endif

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    vec3 fragColor = texture(u_colorMap, v_texCoord).rgb;
    vec3 fragPos = texture(u_positionMap, v_texCoord).rgb;
    vec3 fragNormal = texture(u_normalMap, v_texCoord).rgb;

#ifdef PHONG
    vec4 ss = texture(u_specularMap, v_texCoord);

    FragmentMaterial material = FragmentMaterial(fragColor, ss.rgb, fragNormal, ss.a);

    o_fragColor = phong(material, u_viewPos, fragPos);
#endif

#ifdef PBR
    vec3 mra = texture(u_metallic_roughness_ambientOcclusion_map).rgb;

    FragmentMaterial material = FragmentMaterial(fragColor, fragNormal, mra.r, mra.g, mra.b);

    o_fragColor = pbr(material, u_viewPos, fragPos);
#endif

#ifdef FOG
    vec4 fragCameraPos = u_view * vec4(fragPos, 1.0);
    o_fragColor = fog(o_fragColor, length(fragCameraPos));
#endif

#ifdef BRIGHTNESS
    saveBrightness(o_fragColor);
#endif
}
