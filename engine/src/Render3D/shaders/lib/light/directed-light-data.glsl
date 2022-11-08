/////////////////////////////////////////////////////////////
///////////////// DIRECTED LIGHT DATA BEGIN /////////////////
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
struct DirectedLight {
    int pcf;
    float biasFactor;
    float biasMin;

    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    mat4 spaceMatrix;
    sampler2D shadowMap;
};

/////////////////////////////////////////////////////////////
//////////////////////// DEFINES ////////////////////////////
/////////////////////////////////////////////////////////////
const int c_maxDirectedLightsNumber = 4;

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform int u_directedLightsNumber;
uniform DirectedLight u_directedLights[c_maxDirectedLightsNumber];

/////////////////////////////////////////////////////////////
////////////////// DIRECTED LIGHT DATA END //////////////////
/////////////////////////////////////////////////////////////
