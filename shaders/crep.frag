#version 330 core

in vec2 uv;

uniform sampler2D tex;
uniform vec3 color;

out vec4 fragColor;

void main(void)
{
    fragColor = mix(vec4(color, 1), texture(tex, uv), 0.5);
}
