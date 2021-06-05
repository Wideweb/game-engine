#version 330 core

/////////////////////////////////////////////////////////////
/////////////////////// DECLARATION /////////////////////////
/////////////////////////////////////////////////////////////
void update();
void render();

/////////////////////////////////////////////////////////////
//////////////////////// ATTRIBUTES /////////////////////////
/////////////////////////////////////////////////////////////
layout(location = 0) in vec3 a_startPosition;
layout(location = 1) in vec3 a_position;
layout(location = 2) in vec3 a_velocity;
layout(location = 3) in float a_startTime;

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;
uniform vec3 u_viewPos;

uniform vec3 u_gravity = vec3(0.0, -0.05, 0.0);
uniform float u_time;
uniform float u_deltaTime;
uniform float u_lifetime;

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
out vec3 v_startPosition;
out vec3 v_position;
out vec3 v_velocity;
out float v_startTime;

out vec4 v_color = vec4(0);

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    update();
    render();
}

void update() {
    v_startPosition = a_startPosition;
    v_position = a_position;
    v_velocity = a_velocity;
    v_startTime = a_startTime;

    if (u_time < a_startTime) {
        v_position = vec3(u_model * vec4(a_startPosition, 1.0));
        return;
    }

    float age = u_time - a_startTime;
    if (age > u_lifetime) {
        v_position = vec3(u_model * vec4(a_startPosition, 1.0));
        v_startTime = u_time;
        return;
    }

    v_position += a_velocity * u_deltaTime;
}

void render() {
    float lifePart = (u_time - a_startTime) / u_lifetime;
    float scale = lifePart;
    if (lifePart < 0.5) {
        v_color = mix(vec4(1.0, 0.1, 0.0, 1.0), vec4(1.0, 1.0, 0.0, 1.0), lifePart / 0.5);
    } else {
        scale = 1 - lifePart;
        v_color = mix(vec4(1.0, 1.0, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), (lifePart - 0.5) / 0.5);
    }

    vec4 worldPos = vec4(a_position, 1.0);
    float distanceToCamera = length(u_viewPos - vec3(worldPos));

    gl_PointSize = 100.0 * clamp(1 / distanceToCamera, 0.0, 1.0) * (scale * 2.0);
    gl_Position = u_projection * u_view * worldPos;
}
