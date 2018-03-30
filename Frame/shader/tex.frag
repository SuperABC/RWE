#version 330 core

precision highp float;

#define MAX_LIGHT 28

out vec4 gl_FragColor;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_Coord;
in vec4 v_PosFromLight[MAX_LIGHT];

struct Light {
	vec3 u_lightPos;
	vec3 u_lightDiff;
	vec3 u_lightAmb;
	float u_lightSpec;
};
uniform int u_enLight;
uniform int u_lightNum;
uniform Light u_lightInfo[MAX_LIGHT];

uniform int u_enBump;
uniform sampler2D u_shadowMap[MAX_LIGHT];
uniform sampler2D u_textureMap;
uniform sampler2D u_bumpMap;

uniform vec3 u_eyePos;

float ShadowCalculation(vec4 posFromLight, int idx) {
    vec3 projCoords = posFromLight.xyz / posFromLight.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(u_shadowMap[idx], projCoords.xy).r;
    float currentDepth = projCoords.z;
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_lightInfo[idx].u_lightPos - v_Position);
    float bias = 0.005;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_shadowMap[idx], 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(u_shadowMap[idx], projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
		}
	}
    shadow /= 9.0;
    if(projCoords.z > 1.0) shadow = 0.0;
    return shadow;
}

vec3 LightCalculation(int idx){
    vec3 fragColor = texture2D(u_textureMap, v_Coord).rgb;
    vec3 fragNormal;
	if(u_enBump == 0) {
		fragNormal = normalize(v_Normal);
	}
	else {
		fragNormal = texture2D(u_bumpMap, v_Coord).rgb;
	}

    vec3 lightColor = u_lightInfo[idx].u_lightDiff;
    vec3 ambient = u_lightInfo[idx].u_lightAmb;
    vec3 lightDir = normalize(u_lightInfo[idx].u_lightPos - v_Position);
    float diff = max(dot(lightDir, fragNormal), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 viewDir = normalize(u_eyePos - v_Position);
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(fragNormal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * u_lightInfo[idx].u_lightSpec * lightColor;
    float shadow = ShadowCalculation(v_PosFromLight[idx], idx);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * fragColor;
	return lighting;
}

void main() {
	vec3 lighting = vec3(0.0, 0.0, 0.0);
	if(u_lightNum==0||u_enLight==0) {
		gl_FragColor = vec4(texture2D(u_textureMap, v_Coord).rgb, 1.0);
	}
	else {
		for(int i = 0; i < u_lightNum; i++){
			lighting += LightCalculation(i);
		}
		gl_FragColor = vec4(lighting, 1.0);
	}
}