#version 330 core

precision highp float;

out vec4 gl_FragColor;

in vec3 v_Position;
in vec3 v_Normal;
in vec2 v_Coord;
in vec4 v_PosFromLight;

uniform sampler2D u_shadowMap;
uniform sampler2D u_textureMap;

uniform vec3 u_eyePos;
uniform vec3 u_lightPos;
uniform vec3 u_lightDiff;
uniform vec3 u_lightAmb;
uniform float u_lightSpec;

float ShadowCalculation(vec4 posFromLight) {
    vec3 projCoords = posFromLight.xyz / posFromLight.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(u_shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(u_lightPos - v_Position);
    float bias = 0.005;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(u_shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
		}
	}
    shadow /= 9.0;
    if(projCoords.z > 1.0) shadow = 0.0;
    return shadow;
}


void main() {
    vec3 fragColor = texture2D(u_textureMap, v_Coord).rgb;
    vec3 fragNormal = normalize(v_Normal);
    vec3 lightColor = u_lightDiff;
    vec3 ambient = u_lightAmb;
    vec3 lightDir = normalize(u_lightPos - v_Position);
    float diff = max(dot(lightDir, fragNormal), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 viewDir = normalize(u_eyePos - v_Position);
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(fragNormal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * u_lightSpec * lightColor;
    float shadow = ShadowCalculation(v_PosFromLight);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * fragColor;
    gl_FragColor = vec4(lighting, 1.0);
}