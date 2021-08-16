#version 330 core

layout (location = 0) in vec3 aPos; // input attribute position
layout (location = 1) in vec3 aColor; // input attribute color

out vec3 color; // gets sent to fragment shader

void main() {
    gl_Position = vec4(aPos, 1.0);
    color = aColor;
}
