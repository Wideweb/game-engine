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
layout(location = 1) in vec3 a_startVelocity;
layout(location = 2) in vec3 a_position;
layout(location = 3) in vec3 a_velocity;
layout(location = 4) in float a_startTime;

/////////////////////////////////////////////////////////////
//////////////////////// UNIFORMS ///////////////////////////
/////////////////////////////////////////////////////////////
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;
uniform vec3 u_viewPos;

uniform float u_time;
uniform float u_deltaTime;

uniform float u_lifetime;
uniform float u_sizeFrom;
uniform float u_sizeTo;
uniform vec3 u_colorFrom;
uniform vec3 u_colorTo;
uniform vec3 u_gravity;

uniform int u_draw;

/////////////////////////////////////////////////////////////
///////////////////////// VARYING ///////////////////////////
/////////////////////////////////////////////////////////////
out vec3 v_startPosition;
out vec3 v_startVelocity;
out vec3 v_position;
out vec3 v_velocity;
out float v_startTime;
out float v_distanceToCamera;

out vec4 v_color = vec4(0);

/////////////////////////////////////////////////////////////
////////////////////////// MAIN /////////////////////////////
/////////////////////////////////////////////////////////////
void main() {
    if (u_draw > 0) {
        render();
    } else {
        gl_PointSize = 0.0;
        update();
    }
}

void update() {
    v_startPosition = a_startPosition;
    v_startVelocity = a_startVelocity;
    v_position = a_position;
    v_velocity = a_velocity;
    v_startTime = a_startTime;

    if (u_time < a_startTime) {
        v_position = vec3(u_model * vec4(a_startPosition, 1.0));
        return;
    }

    float age = u_time - a_startTime;
    float deltaTime = u_deltaTime;

    if (age > u_lifetime) {
        deltaTime = age - u_lifetime;

        v_position = vec3(u_model * vec4(a_startPosition, 1.0));
        v_velocity = a_startVelocity;
        v_startTime = u_time - deltaTime;
    }

    vec3 velocity = v_velocity + u_gravity * deltaTime;

    v_velocity = velocity;
    v_position += velocity * deltaTime;
}

void render() {
    if (u_time < a_startTime) {
        gl_PointSize = 0;
        return;
    }

    float lifePart = (u_time - a_startTime) / u_lifetime;
    vec4 worldPos = vec4(a_position, 1.0);
    v_distanceToCamera = length(u_viewPos - vec3(worldPos));

    v_color = vec4(mix(u_colorFrom, u_colorTo, lifePart), 1.0);
    gl_PointSize = mix(u_sizeFrom, u_sizeTo, lifePart) * clamp(1 / v_distanceToCamera, 0.0, 1.0);
    gl_Position = u_projection * u_view * worldPos;
}
