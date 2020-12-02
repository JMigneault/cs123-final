#version 330 core

layout(location = 0) in vec2 position; // Screenspace coords
layout(location = 5) in vec2 uvCoords;

/*
uniform sampler2D texture1Un;
uniform sampler2D texture2Un;

out sampler2D texture1;
out sampler2D texture2;
*/ // NOTE: not necessary, frag can just take the textures (I think)

out vec2 uv;

void main(void)
{
    gl_Position = vec4(position, 0.0, 1.0);
    uv = uvCoords;
}
