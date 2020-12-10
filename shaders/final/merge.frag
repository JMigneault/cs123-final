#version 330 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D crepescular;
uniform sampler2D scene;

uniform float phongWeight;

void main(void)
{
    fragColor = texture(scene, uv) * phongWeight + texture(crepescular, uv);
}
