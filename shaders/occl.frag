#version 330 core

out vec3 fragColor;

void main(void)
{
    // NOTE: render in inverse color (so black in unoccluded)
    // thus, occluding geometry is rendered in full black
    fragColor = vec3(1, 1, 1);
}
