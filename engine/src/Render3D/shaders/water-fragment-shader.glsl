#version 330 core

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const float c_waveStrength = 0.02;
const float c_near = 0.01;
const float c_far = 100.0;

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform sampler2D u_colorMap;
uniform sampler2D u_depthMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_dudvMap;
uniform float u_moveFactor;
uniform vec3 u_lightDir;

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
in vec4 v_clipSpace;
in vec2 v_texCoord;
in vec3 v_viewDir;

/////////////////////////////////////////////////////////////
/////////////////////////// OUT /////////////////////////////
/////////////////////////////////////////////////////////////
out vec4 o_fragColor;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    vec2 ndc = (v_clipSpace.xy / v_clipSpace.w) / 2.0 + 0.5;

    vec2 reflectTexCoord = vec2(ndc.x, -ndc.y);

    float depth = texture(u_depthMap, reflectTexCoord).r;
    float floorDistance = 2.0 * c_near * c_far / (c_far + c_near - depth * (c_far - c_near));
    float waterDistance = 2.0 * c_near * c_far / (c_far + c_near - gl_FragCoord.z * (c_far - c_near));
    float waterDepth = floorDistance - waterDistance;

    vec2 distortion1 =
        (texture(u_dudvMap, vec2(v_texCoord.x + u_moveFactor, v_texCoord.y)).rg * 2.0 - 1.0) * c_waveStrength;
    vec2 distortion2 =
        (texture(u_dudvMap, vec2(-v_texCoord.x + u_moveFactor, v_texCoord.y + u_moveFactor)).rg * 2.0 - 1.0) *
        c_waveStrength;

    vec2 totalDistortion = distortion1 + distortion2;

    reflectTexCoord += totalDistortion;
    reflectTexCoord.x = clamp(reflectTexCoord.x, 0.001, 0.999);
    reflectTexCoord.y = clamp(reflectTexCoord.y, -0.999, -0.001);

    float refractFactor = dot(normalize(v_viewDir), vec3(0, 1, 0));
    refractFactor = pow(refractFactor, 0.5);

    vec4 reflectColor = vec4(texture(u_colorMap, reflectTexCoord).rgb, refractFactor);
    reflectColor = mix(reflectColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2);

    vec4 normalMapColor = texture(u_normalMap, reflectTexCoord);
    vec3 normal = vec3(normalMapColor.r * 2 - 1, normalMapColor.g, normalMapColor.b * 2 - 1);
    normal = normalize(normal);

    vec3 reflectedLightDir = reflect(-u_lightDir, normal);
    vec3 halfwayDir = normalize(u_lightDir + v_viewDir);
    float specular = pow(max(dot(normal, halfwayDir), 0.0), 128);
    vec3 specularColor = vec3(1.0f, 1.0f, 1.0f) * specular;

    o_fragColor = reflectColor + vec4(specularColor, 0.0);
    o_fragColor.a = min(o_fragColor.a, clamp(waterDepth / 5.0, 0.0, 1.0));
}
