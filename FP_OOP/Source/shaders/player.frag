#version 330 core

uniform sampler2D texture_pos;

in vec2 pass_texture_coords;

out vec4 color;

void main() {
    color = texture(texture_pos, pass_texture_coords);
    if (color.w < 1.0) {
        discard;
    }
}
