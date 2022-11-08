#version 330 core

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform sampler2D u_colorBuffer;
uniform bool u_horizontal;
uniform float u_weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
in vec2 v_texCoord;

/////////////////////////////////////////////////////////////
/////////////////////////// OUT /////////////////////////////
/////////////////////////////////////////////////////////////
out vec4 o_fragColor;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
// void main() {
//     vec2 texOffset = 1.0 / textureSize(u_colorBuffer, 0);
//     // вклад текущего фрагмента
//     // vec3 result = texture(u_colorBuffer, v_texCoord).rgb * u_weight[0];

//     if (u_horizontal) {
//         texOffset.y = 0.0;
//     } else {
//         texOffset.x = 0.0;
//     }

//     vec4 result = vec4(0.0);
//     result += texture(u_colorBuffer, v_texCoord - texOffset * 3.0) * 0.07;
// 	result += texture(u_colorBuffer, v_texCoord - texOffset * 2.0) * 0.13;
// 	result += texture(u_colorBuffer, v_texCoord - texOffset * 1.0) * 0.19;
// 	result += texture(u_colorBuffer, v_texCoord) * 0.22;
// 	result += texture(u_colorBuffer, v_texCoord + texOffset * 1.0) * 0.19;
// 	result += texture(u_colorBuffer, v_texCoord + texOffset * 2.0) * 0.13;
// 	result += texture(u_colorBuffer, v_texCoord + texOffset * 3.0) * 0.07;

//     o_fragColor = result;
// }

void main() {
    vec2 texOffset = 1.0 / textureSize(u_colorBuffer, 0);
    // вклад текущего фрагмента
    vec3 result = texture(u_colorBuffer, v_texCoord).rgb * u_weight[0];
    if (u_horizontal) {
        for (int i = 1; i < 5; ++i) {
            result += texture(u_colorBuffer, v_texCoord + vec2(texOffset.x * i, 0.0)).rgb * u_weight[i];
            result += texture(u_colorBuffer, v_texCoord - vec2(texOffset.x * i, 0.0)).rgb * u_weight[i];
        }
    } else {
        for (int i = 1; i < 5; ++i) {
            result += texture(u_colorBuffer, v_texCoord + vec2(0.0, texOffset.y * i)).rgb * u_weight[i];
            result += texture(u_colorBuffer, v_texCoord - vec2(0.0, texOffset.y * i)).rgb * u_weight[i];
        }
    }
    o_fragColor = vec4(result, 1.0);
}
