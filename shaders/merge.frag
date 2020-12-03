#version 330 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D crepescular;
uniform sampler2D scene;


void main(void)
{
    // TODO: figure out how to render get 2 textures from FBOs as uniform (12/3)

    // NOTE: render in inverse color (so black in unoccluded)
    // thus, occluding geometry is rendered in full black
    fragColor = texture(scene, uv); // + texture(crepescular, uv);
}
