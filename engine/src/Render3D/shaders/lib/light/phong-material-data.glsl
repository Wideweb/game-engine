/////////////////////////////////////////////////////////////
/////////////////// PHONG MATERIAL BEGIN ////////////////////
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
struct Material {
    @Color
    @Editable("Diffuse Color")
    vec3 diffuseColor;
    
    @Bool
    @Editable("Use Diffuse Map")
    int diffuseUseTexture;
    
    @Texture
    @Editable("Diffuse Map")
    sampler2D diffuseMap;
    
    @Color
    @Editable("Diffuse Color")
    vec3 specularColor;

    @Bool
    @Editable("Use Specular Map")
    int specularUseTexture;
    
    @Texture
    @Editable("Specular Map")
    sampler2D specularMap;

    @Bool
    @Editable("Use Normal Map")
    int normalUseTexture;
    
    @Texture
    @Editable("Normal Map")
    sampler2D normalMap;
    
    @Range(0, 128)
    @Editable("Shininess")
    float shininess;
};

struct FragmentMaterial {
    vec3 diffuse;
    vec3 specular;
    vec3 normal;
    float shininess;
};

FragmentMaterial getFragmentMaterial(vec2 texCoord, vec3 normal);

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
@Editable("Material")
uniform Material u_material;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
FragmentMaterial getFragmentMaterial(vec2 texCoord, vec3 normal) {
    vec3 fragDiffuse = u_material.diffuseColor;
    if (u_material.diffuseUseTexture > 0) {
        fragDiffuse = texture(u_material.diffuseMap, texCoord).rgb;
    }

    vec3 fragSpecular = u_material.specularColor;
    if (u_material.specularUseTexture > 0) {
        fragSpecular = texture(u_material.specularMap, texCoord).rgb;
    }

    return FragmentMaterial(fragDiffuse, fragSpecular, normal, u_material.shininess);
}

/////////////////////////////////////////////////////////////
//////////////////// PHONG MATERIAL END /////////////////////
/////////////////////////////////////////////////////////////
