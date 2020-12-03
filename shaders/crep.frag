#version 330 core

in vec2 positionScreen;
in vec2 uv;

uniform sampler2D tex;

out vec4 fragColor;

void main(void)
{


    vec2 lightUV = vec2(0.5, 0.5);

    float exposure = .35f; // NOTE: difference between weight and exposure??
    float decay = 0.987f;
    float weight = 1.0f;

    int n = 100;
    float overN = 1.0f/n;

    vec4 sum = vec4(0, 0, 0, 0);

    vec2 currentSample = lightUV;

    vec2 dSample = (uv - lightUV) * overN; // TODO: off by 1 error?

    for (int i = 0; i < n; i++) {
        sum += pow(decay, i) * weight * texture(tex, currentSample) * overN;
        currentSample += dSample;
    }

    sum *= exposure;

    fragColor = sum + texture(tex, uv);

}
