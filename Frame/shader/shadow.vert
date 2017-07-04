#version 400

layout(location=0)in vec3 a_Position;

uniform int u_LightMode;

uniform mat4 u_PerspectMatrix;
uniform mat4 u_ViewMatrix;

out vec3 v_Position;

void main() {
    if(u_LightMode == 1)
        gl_Position = u_ViewMatrix * vec4(a_Position, 1.0);
    else
        gl_Position = u_PerspectMatrix * u_ViewMatrix * vec4(a_Position, 1.0);
    v_Position = gl_Position.xyz;
}