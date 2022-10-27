#version 330 core

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform sampler2D u_colorBuffer;
uniform vec3 u_viewPos;
uniform float u_farPlane;

/////////////////////////////////////////////////////////////
//////////////////////// VARYING ////////////////////////////
/////////////////////////////////////////////////////////////
in vec2 v_vertexPosition;
in vec3 v_fragWorldPosition;
in vec3 v_fragCameraPosition;
in vec3 v_normal;

/////////////////////////////////////////////////////////////
/////////////////////////// OUT /////////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) out vec4 o_fragColor;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    vec2 posInGrid = floor(v_vertexPosition);
    vec2 posInCell = fract(v_vertexPosition);
    vec2 texCoord = posInGrid / textureSize(u_colorBuffer, 0);

    vec4 cellColor = vec4(texture(u_colorBuffer, texCoord).rgb, 1.0);

    vec3 viewDir = normalize(u_viewPos - v_fragWorldPosition);
    float angleFactor = max(pow(dot(v_normal, viewDir), 2), 0.1);

    float distanceToCamera = length(v_fragCameraPosition);
    float farPlaneFactor = clamp(1.0 - distanceToCamera / u_farPlane, 0.0, 1.0);

    float border = 0.01 / farPlaneFactor;
    float fade = 0.01 / angleFactor / farPlaneFactor;

    float borderToCell = 1.0;
    borderToCell *= smoothstep(border - fade, border + fade, posInCell.x);
    borderToCell *= 1.0 - smoothstep(1.0 - border - fade, 1.0 - border + fade, posInCell.x);
    borderToCell *= smoothstep(border - fade, border + fade, posInCell.y);
    borderToCell *= 1.0 - smoothstep(1.0 - border - fade, 1.0 - border + fade, posInCell.y);

    vec4 borderColor = vec4(0.0, 0.0, 0.0, 1.0);
    borderColor = mix(cellColor, borderColor, farPlaneFactor);

    o_fragColor = mix(borderColor, cellColor, borderToCell);
    o_fragColor.a *= smoothstep(0.0, 0.2, farPlaneFactor);
}
