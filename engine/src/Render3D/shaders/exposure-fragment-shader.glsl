#version 330 core

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform sampler2D u_brightness;
uniform sampler2D u_lastExposure;
uniform float u_sceneBrightness;
uniform vec2 u_exposureMinMax;

/////////////////////////////////////////////////////////////
/////////////////////////// OUT /////////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) out float o_fragColor;

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    float center = textureLod(u_brightness, vec2(0.5, 0.5), 6).r;
	float c0 = textureLod(u_brightness, vec2(0.25, 0.25), 6).r;
	float c1 = textureLod(u_brightness, vec2(0.75, 0.25), 6).r;
	float c2 = textureLod(u_brightness, vec2(0.75, 0.75), 6).r;
	float c3 = textureLod(u_brightness, vec2(0.25, 0.75), 6).r;
	float brightness = center * 0.4 + c0 * 0.15 + c1 * 0.15 + c2 * 0.15 + c3 * 0.15;

	float targetExposure = clamp(0.5 * u_sceneBrightness / brightness, u_exposureMinMax.x, u_exposureMinMax.y);
	float lastExposure = texture(u_lastExposure, vec2(0.5, 0.5)).r;
	float newExposure = mix(lastExposure, targetExposure, 0.05);
	o_fragColor = newExposure;
}
