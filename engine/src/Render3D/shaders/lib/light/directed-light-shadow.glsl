/////////////////////////////////////////////////////////////
/////////////// DIRECTED LIGHT SHADOW BEGIN /////////////////
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
float directedLightShadowCalculation(DirectedLight light, vec3 fragPos, float bias);

/////////////////////////////////////////////////////////////
////////////////// DIRECTED LIGHT SHADOW ////////////////////
/////////////////////////////////////////////////////////////
float directedLightShadowCalculation(DirectedLight light, vec3 fragPos, float bias) {
    vec4 fragPosInLightSpace = light.spaceMatrix * vec4(fragPos, 1.0);
    vec3 coords = fragPosInLightSpace.xyz / fragPosInLightSpace.w;
    coords = coords * 0.5 + 0.5;
    float currentDepth = coords.z;

    // float closestDepth = texture(light.shadowMap, coords.xy).r;
    // float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    if (currentDepth > 1.0) {
        return 0.0;
    }

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(light.shadowMap, 0);
    for (int x = -light.pcf; x <= light.pcf; ++x) {
        for (int y = -light.pcf; y <= light.pcf; ++y) {
            float pcfDepth = texture(light.shadowMap, coords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= (light.pcf * 2 + 1) * (light.pcf * 2 + 1);

    return shadow;
}

/////////////////////////////////////////////////////////////
///////////////// DIRECTED LIGHT SHADOW END /////////////////
/////////////////////////////////////////////////////////////
