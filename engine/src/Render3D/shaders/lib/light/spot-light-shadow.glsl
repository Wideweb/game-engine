/////////////////////////////////////////////////////////////
//////////////// PHONG LIGHT SHADOW BEGIN ///////////////////
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const vec3 c_sampleOffsetDirections[20] = vec3[](
    vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1),
    vec3(-1, 1, -1), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, 0, 1), vec3(-1, 0, 1),
    vec3(1, 0, -1), vec3(-1, 0, -1), vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1));

float spotLightShadowCalculation(SpotLight light, vec3 fragPos, float bias);

/////////////////////////////////////////////////////////////
//////////////////// SPOT LIGHT SHADOW //////////////////////
/////////////////////////////////////////////////////////////
float spotLightShadowCalculation(SpotLight light, vec3 fragPos, float bias) {
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;

    for (int i = 0; i < light.pcfSamples; ++i) {
        float closestDepth =
            texture(light.shadowMap, fragToLight + c_sampleOffsetDirections[i] * light.pcfDiskRadius).r;
        closestDepth *= light.farPlane;
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(light.pcfSamples);

    return shadow;
}

/////////////////////////////////////////////////////////////
////////////////// PHONG LIGHT SHADOW END ///////////////////
/////////////////////////////////////////////////////////////
