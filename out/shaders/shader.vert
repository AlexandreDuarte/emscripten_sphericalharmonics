#version 300 es

attribute vec3 pos;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(pos, 1);
}