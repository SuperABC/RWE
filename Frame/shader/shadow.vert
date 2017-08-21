#version 330 core

layout (location = 0) in vec3 a_Pos;


uniform mat4 u_lightMatrix;
uniform mat4 u_modelMatrix;


void main() {

    gl_Position = u_lightMatrix * u_modelMatrix * vec4(a_Pos, 1.0);

}
