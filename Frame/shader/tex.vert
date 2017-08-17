#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_Coord;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_Coord;
out vec4 v_PosFromLight;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_lightMatrix;

void main() {
    v_Position = a_Position;
    v_Normal = a_Normal;
    v_Coord = a_Coord;

    v_PosFromLight = u_lightMatrix * vec4(v_Position, 1.0);
    gl_Position = u_projection * u_view * vec4(a_Position, 1.0);
}
