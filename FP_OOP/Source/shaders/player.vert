#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coords;

out vec2 pass_texture_coords;

uniform mat4 mvp;

void main() {
    pass_texture_coords = texture_coords;
    gl_Position = mvp * vec4(position, 1.0);
}
