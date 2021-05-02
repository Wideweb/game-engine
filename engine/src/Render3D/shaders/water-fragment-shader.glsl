#version 330 core
out vec4 FragColor;

in vec4 ClipSpace;
in vec2 TexCoord;
in vec3 ViewDir;

uniform sampler2D colorMap;
uniform sampler2D depthMap;
uniform sampler2D normalMap;
uniform sampler2D dudvMap;

uniform float moveFactor;
uniform vec3 lightDir;

const float WAVE_STRENGTH = 0.02;

void main() {
    vec2 ndc = (ClipSpace.xy / ClipSpace.w) / 2.0 + 0.5;

    vec2 reflectTexCoord = vec2(ndc.x, -ndc.y);

    float near = 0.01;
    float far = 100.0f;
    float depth = texture(depthMap, reflectTexCoord).r;
    float floorDistance = 2.0 * near * far / (far + near - depth * (far - near));
    float waterDistance = 2.0 * near * far / (far + near - gl_FragCoord.z * (far - near));
    float waterDepth = floorDistance - waterDistance;

    vec2 distortion1 = (texture(dudvMap, vec2(TexCoord.x + moveFactor, TexCoord.y)).rg * 2.0 - 1.0) * WAVE_STRENGTH;
    vec2 distortion2 = (texture(dudvMap, vec2(-TexCoord.x + moveFactor, TexCoord.y + moveFactor)).rg * 2.0 - 1.0) * WAVE_STRENGTH;

    vec2 totalDistortion = distortion1 + distortion2;

    reflectTexCoord += totalDistortion;
    reflectTexCoord.x = clamp(reflectTexCoord.x, 0.001, 0.999);
    reflectTexCoord.y = clamp(reflectTexCoord.y, -0.999, -0.001);

    float refractFactor = dot(normalize(ViewDir), vec3(0, 1, 0));
    refractFactor = pow(refractFactor, 0.5);

    vec4 reflectColor = vec4(texture(colorMap, reflectTexCoord).rgb, refractFactor);
    reflectColor = mix(reflectColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2);

    vec4 normalMapColor = texture(normalMap, reflectTexCoord);
    vec3 normal = vec3(normalMapColor.r * 2 - 1, normalMapColor.g, normalMapColor.b * 2 - 1);
    normal = normalize(normal);

    vec3 reflectedLightDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + ViewDir);
    float specular = pow(max(dot(normal, halfwayDir), 0.0), 128);
    vec3 specularColor = vec3(1.0f, 1.0f, 1.0f) * specular;

    FragColor = reflectColor + vec4(specularColor, 0.0);
    FragColor.a = min(FragColor.a, clamp(waterDepth / 5.0, 0.0, 1.0));
}
