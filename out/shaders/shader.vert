#version 300 es

precision highp float;

layout(location=0) in vec3 pos;

layout(location=1) in float amp;

uniform vec4 color1;
uniform vec4 color2;

uniform mat4 mvp;

out vec4 switchColor;

void main() {

    bool amp_negative = (amp<0.0f);

    vec3 final_pos;

    if (amp_negative) {
        switchColor = color2;
        final_pos = -amp * pos;
    } else {
        switchColor = color1;
        final_pos = amp * pos;
    }

    gl_Position = mvp * vec4(final_pos, 1);
}