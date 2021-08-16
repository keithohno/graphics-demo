#version 330 core

in vec3 color; // input color from vertex shader

out vec4 fColor; // output fragment color

void main() {
    fColor = vec4(color, 1.0);
}