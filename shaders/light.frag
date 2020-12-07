#version 330 core

in vec2 positionScreen;
in vec2 uv;

uniform sampler2D tex;
uniform vec3 color;

out vec4 fragColor;

void main(void)
{
    vec2 lightPos = vec2(0, .5);

    float s = length(lightPos - positionScreen);

    float B = 2.5f; // 15.0f;
    vec4 L0 = .7 * vec4(1, 1, 1, 1);

    vec4 lightColor = L0 * exp(-B * s); // L0 * (s < .3 ? 1.0 : 0.0);
    // TODO: why is v coord being flipped??
    vec4 occlusionColor = texture(tex, uv);

    fragColor = (lightColor + vec4(color, 1)) * (1 - occlusionColor);

    /*
    // constants
    float L0 = 1.0f;
    float Esun = 1.0f;
    float Br = 0.5f;
    float Bm = 0.5f;
    float g = 1.0f;
    float PI = 3.1415;

    // calculated
    float s = length(lightPos - positionScreen);

    vec3 eyePos = vec3(0, 0, 1);
    vec3 v1 = normalize(vec3(positionScreen, 0) - eyePos);
    vec3 v2 = normalize(vec3(lightPos, 0) - eyePos);
    float theta = acos(dot(v1, v2));

    float Fex = exp(-(Br + Bm) * s);

    float Lin = ; */

    /*
    // constants
    float L = 2.f; // larger makes distance term larger
    float Bex = 5f; // larger makes exp drop off faster
    float Bsc = 1.0f; // ??
    float Esun = 0.3f; // larger makes angular term larger


    float intensity = L * exp(-Bex * S) + (1 / Bex) * Esun * Bsc * theta * (1 - exp(-Bex * S));

    vec4 crepColor;
    if (intensity > 1.0) {
        crepColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        crepColor = vec4(intensity, intensity, intensity, 1.0);
    }

    fragColor = crepColor + texture(tex, uv); */
}
