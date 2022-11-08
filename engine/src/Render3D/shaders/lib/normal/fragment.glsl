/////////////////////////////////////////////////////////////
////////////////// NORMAL MAPPING BEGIN /////////////////////
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
vec3 getNormal();

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
#ifdef NOMRAL_MAPPING
in mat3 v_TBN;
#endif

in vec3 v_normal;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
vec3 getNormal() {
#ifdef NOMRAL_MAPPING

    if (u_material.normalUseTexture <= 0) {
        return v_normal;
    }

    vec3 normal = texture(u_material.normalMap, v_texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(v_TBN * normal);
    return normal;

#else

    return v_normal;

#endif 
}

/////////////////////////////////////////////////////////////
/////////////////// NORMAL MAPPING END //////////////////////
/////////////////////////////////////////////////////////////
