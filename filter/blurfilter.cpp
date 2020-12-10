#include "blurfilter.h"
#include "helpers.h"

BlurFilter::BlurFilter()
{
}

float BlurFilter::gaussian(float x, float sigma) {
    float exp = -1 * x * x / (2 * sigma * sigma);
    float res = (1 / glm::sqrt(2 * glm::pi<float>() * sigma)) * glm::pow(glm::e<float>(), exp);
    return res;
}

void BlurFilter::filter(Canvas2D *canvas, int radius) {
    int height = canvas->height();
    int width = canvas->width();

    int kLength = 2 * radius + 1;
    float kernel[kLength];
    float sigma = radius / 3.0f;
    for (int x = -radius; x < radius + 1; x++) {
        int i = x + radius;
        kernel[i] = gaussian(x, sigma);
    }

    // divide the 2nd kernel by 4 to maintain brightness
    Helpers::normalizeKernel(kernel, kLength);

    // setup buffers to hold partially filtered results
    std::vector<Helpers::rgbFloat> buffer1(height * width);
    std::vector<Helpers::rgbFloat> buffer2(height * width);

    Helpers::rgbToRgbFloat(buffer1.data(), canvas->data(), height * width);

    // vertical blur
    Helpers::conv2D(buffer2.data(), buffer1.data(), width, height, kernel, kLength, 1);

    // horizontal blur
    Helpers::conv2D(buffer1.data(), buffer2.data(), width, height, kernel, 1, kLength);

    Helpers::rgbFloatToRgb(canvas->data(), buffer1.data(), width * height);
}

