#version 330 core

in vec2 positionScreen;
in vec2 uv;

uniform sampler2D tex;

uniform vec2 screenSunPos;

uniform float exposure;
uniform float decay;
uniform float weight;
uniform float density;

out vec4 fragColor;

vec2 screenToUV(vec2 screenPos) {
    return (screenPos + 1.0) * 0.5f;
}

void main(void)
{

    int NUM_SAMPLES = 200;

    float decayFactor = 1.0f;
    int n = 0;
    vec3 effectColor = vec3(0, 0, 0);
    vec4 initialColor = texture(tex, uv);

    vec2 dSample = (screenSunPos - positionScreen) * (density / NUM_SAMPLES);

    vec2 currentSample = positionScreen + dSample;
    vec2 currentUV = screenToUV(currentSample);

    while (n < NUM_SAMPLES
           && currentUV.x >= 0.0f && currentUV.x <= 1.0f
           && currentUV.y >= 0.0f && currentUV.y <= 1.0f) {
        vec4 sampleColor = texture(tex, currentUV);
        effectColor += (sampleColor * decayFactor).xyz;
        decayFactor *= decay;
        n += 1;
        currentSample += dSample;
        currentUV = screenToUV(currentSample);
    }

    fragColor = exposure * (initialColor + vec4((effectColor * (weight / n)).xyz, 1));
}


/*
float4 main(float2 texCoord : TEXCOORD0) : COLOR0 {

    // Calculate vector from pixel to light source in screen space.
    half2 deltaTexCoord = (texCoord - ScreenLightPos.xy);

    // Divide by number of samples and scale by control factor.
    deltaTexCoord *= 1.0f / NUM_SAMPLES * Density;

    // Store initial sample.
    half3 color = tex2D(frameSampler, texCoord);

    // Set up illumination decay factor.
    half illuminationDecay = 1.0f;

    // Evaluate summation from Equation 3 NUM_SAMPLES iterations.
    for (int i = 0; i < NUM_SAMPLES; i++)   {
        // Step sample location along ray.
        texCoord -= deltaTexCoord;

        // Retrieve sample at new location.
        half3 sample = tex2D(frameSampler, texCoord);

        // Apply sample attenuation scale/decay factors.
        sample *= illuminationDecay * Weight;

        // Accumulate combined color.
        color += sample;

        // Update exponential decay factor.
        illuminationDecay *= Decay;

   }
   // Output final color with a further scale control factor.
   return float4( color * Exposure, 1);
}

 */
