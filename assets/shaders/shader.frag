#version 300 es

precision highp float;

in vec4 switchColor;
out vec4 color;

void main() {

    color = switchColor;

}