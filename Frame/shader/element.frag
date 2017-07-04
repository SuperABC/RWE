#version 400

in vec3 v_Position;
in vec3 v_Color;
in vec3 v_Normal;

in vec4 v_PosFromLight;

uniform int u_LightMode;
uniform vec3 u_LightPos;
uniform vec3 u_LightDir;
uniform vec3 u_LightDiffuse;
uniform vec3 u_LightAmbient;
uniform vec3 u_LightSpecular;

uniform sampler2D u_ShadowMap;

void main() {
    float visibility = 1.0;
    vec3 shadowCoord = (v_PosFromLight.xyz/v_PosFromLight.w)/2.0 + 0.5;
    vec4 rgbaDepth = texture2D(u_ShadowMap, shadowCoord.xy);
    float depth = rgbaDepth.r;
    if(shadowCoord.x <= 0.01 || shadowCoord.x >= 0.99 || shadowCoord.y <= 0.01 || shadowCoord.y >= 0.99)
        visibility = 1.0;
    else
        visibility = (shadowCoord.z > depth + 0.005) ? 0.0 : 1.0;

    if(u_LightMode == 0)
        gl_FragColor = vec4(v_Color, 1.0);
    else if(u_LightMode == 1){
        vec3 normal = normalize(v_Normal);
        vec3 dir = normalize(u_LightDir);
        float product= max(dot(dir, -normal), 0.0);
        vec3 diffuse = u_LightDiffuse * v_Color.rgb * product * visibility;
        vec3 ambient = u_LightAmbient * v_Color.rgb;
        gl_FragColor += vec4(diffuse + ambient, 1.0);
    }
    else if(u_LightMode == 2){
        vec3 normal = normalize(v_Normal);
        vec3 dir = normalize(v_Position - u_LightPos);
        float product= max(dot(dir, -normal), 0.0);
        vec3 diffuse = u_LightDiffuse * v_Color.rgb * product * visibility;
        vec3 ambient = u_LightAmbient * v_Color.rgb;
        gl_FragColor += vec4(diffuse + ambient, 1.0);
    }
}