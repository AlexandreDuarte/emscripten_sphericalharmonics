#version 300 es

precision highp float;

layout(location=0) in vec3 pos;

layout(location=1) in float sC;

uniform mat4 mvp;

out float switchColor;

void main() {
    switchColor = sC;
    gl_Position = mvp * vec4(pos, 1);
}