#version 400

in vec3 v_Position;

void main() {
    gl_FragColor = vec4(v_Position.z, 0.0, 0.0, 0.0);
    //gl_FragColor = vec4(v_Position.x, v_Position.y, v_Position.z, 0.0);
}