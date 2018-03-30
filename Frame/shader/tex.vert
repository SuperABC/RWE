#version 330 core

#define MAX_LIGHT 28

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_Coord;

out vec3 v_Position;
out vec3 v_Normal;
out vec2 v_Coord;
out vec4 v_PosFromLight[MAX_LIGHT];

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_modelMatrix;
uniform mat4 u_normalMatrix;

uniform int u_lightNum;
uniform mat4 u_lightsMatrix[MAX_LIGHT];

void main() {
    v_Position = (u_modelMatrix * vec4(a_Position, 1.0)).xyz;
    v_Normal =  (u_normalMatrix * vec4(a_Normal, 1.0)).xyz;
    v_Coord = a_Coord;
	
	for(int i = 0; i < u_lightNum; i++) {
		v_PosFromLight[i] = u_lightsMatrix[i] * vec4(v_Position, 1.0);
	}
    gl_Position = u_projection * u_view * vec4(v_Position, 1.0);
}
