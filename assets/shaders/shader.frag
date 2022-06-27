#version 300 es

precision highp float;

uniform vec4 color1;
uniform vec4 color2;

in float switchColor;
out vec4 color;

void main() {

    if(switchColor > 0.0f) {
        color = color1;
    } else {
        color = color2;
    }

}