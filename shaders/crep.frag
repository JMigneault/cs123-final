#version 330 core

in vec2 positionScreen;
in vec2 uv;

uniform sampler2D tex;
uniform float weightParam;

out vec4 fragColor;

void main(void)
{

    vec2 lightUV = vec2(.5, .75);

    // fragColor = texture(tex, uv) + vec4(.2 > length(lightUV - uv) ? 1 : 0, 0, 0, 1);
    // fragColor = ;
    // return;

    float exposure = 0.5f; // NOTE: difference between weight and exposure??
    float decay = 1.0f;

    int n = 1000;
    float weight = weightParam / n;

    vec4 sum = vec4(0, 0, 0, 0);
    vec4 color = texture(tex, uv);

    vec2 currentSample = uv;
    float decayFactor = 1.0f;

    vec2 dSample = (lightUV - uv) * (1.0 / n); // TODO: off by 1 error?

    for (int i = 0; i < n; i++) {
        currentSample += dSample;
        vec4 sampleColor = texture(tex, currentSample);
        color += sampleColor * weight * decayFactor;
        decayFactor *= decay;
    }

    fragColor = color * exposure;
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
