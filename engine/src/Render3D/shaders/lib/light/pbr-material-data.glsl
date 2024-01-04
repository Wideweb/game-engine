/////////////////////////////////////////////////////////////
//////////////////// PBR MATERIAL BEGIN /////////////////////
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
struct Material {
    vec3 diffuseColor;
    int diffuseUseTexture;
    sampler2D diffuseMap;

    int normalUseTexture;
    sampler2D normalMap;

    float metallic;
    int metallicUseTexture;
    sampler2D metallicMap;

    float roughness;
    int roughnessUseTexture;
    sampler2D roughnessMap;

    float mbientOcclusion;
    int ambientUseTexture;
    sampler2D ambientOcclusionMap;
};

struct FragmentMaterial {
    vec3 albedo;
    vec3 normal;
    float metallic;
    float roughness;
    float ambientOcclusion;
};

FragmentMaterial getFragmentMaterial(vec2 texCoord, vec3 normal);

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
@Editable("Material") uniform Material u_material;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
FragmentMaterial getFragmentMaterial(vec2 texCoord, vec3 normal) {
    // vec3 albedo = pow(texture(u_material.diffuse, texCoord).rgb, vec3(2.2));
    vec3 fragDiffuse = u_material.diffuseColor;
    if (u_material.diffuseUseTexture > 0) {
        fragDiffuse = texture(u_material.diffuseMap, texCoord).rgb;
    }

    float fragMetallic = u_material.metallic;
    if (u_material.metallicUseTexture > 0) {
        fragMetallic = texture(u_material.metallicMap, texCoord).r;
    }

    float fragRoughness = u_material.roughness;
    if (u_material.roughnessUseTexture > 0) {
        fragRoughness = texture(u_material.roughnessMap, texCoord).r;
    }

    float fragAmbientOcclusion = u_material.ambientOcclusion;
    if (u_material.ambientOcclusionUseTexture > 0) {
        fragAmbientOcclusion = texture(u_material.ambientOcclusionMap, texCoord).r;
    }

    return FragmentMaterial(fragDiffuse, normal, fragMetallic, fragRoughness, fragAmbientOcclusion);
}

/////////////////////////////////////////////////////////////
///////////////////// PBR MATERIAL END //////////////////////
/////////////////////////////////////////////////////////////