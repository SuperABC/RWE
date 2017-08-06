#version 330 core

layout (location = 0) in vec3 a_Pos;



uniform mat4 u_lightMatrix;



void main() {

    gl_Position = u_lightMatrix * vec4(a_Pos, 1.0);

}
