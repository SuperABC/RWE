#version 400

layout(location=0)in vec3 a_Position;
layout(location=1)in vec3 a_Color;
layout(location=2)in vec3 a_Normal;

uniform int u_StaticView;

uniform mat4 u_PerspectMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_MatrixFromLight;

out vec3 v_Position;
out vec3 v_Color;
out vec3 v_Normal;

out vec4 v_PosFromLight;

void main() {
    if(u_StaticView == 1)
        gl_Position = u_PerspectMatrix * vec4(a_Position, 1.0);
    else
        gl_Position = u_PerspectMatrix * u_ViewMatrix * vec4(a_Position, 1.0);
    v_Position = a_Position;
    v_Color = a_Color;
    v_Normal = a_Normal;
    v_PosFromLight = u_MatrixFromLight * vec4(a_Position, 1.0);
}