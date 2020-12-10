#include "scalefilter.h"
#include "helpers.h"

ScaleFilter::ScaleFilter()
{

}

float inline sinc (float x) {
    return x == 0 ? 1 : glm::sin(glm::pi<float>() * x) / (glm::pi<float>() * x);
}

float inline tri(float x) {
    if (x < 0) x = -x;

    if (x > 1) return 0;

    return 1 - x;
}

int inline adjustIndex(int i, int bound) {

    if (i < 0 || i >= bound) {
        return i >= bound ? 2 * bound - i - 1 : -i - 1;
    }
    return i;
}

void ScaleFilter::filter(Canvas2D *canvas, float scaleX, float scaleY) {

    if (scaleX >= 1) {
        scaleUpWidth(canvas, scaleX);
    } else {
        scaleDownWidth(canvas, scaleX);
    }

    if (scaleY >= 1) {
        scaleUpHeight(canvas, scaleY);
    } else {
        scaleDownHeight(canvas, scaleY);
    }

}

void ScaleFilter::scaleUpWidth(Canvas2D *canvas, float a) {
    
    int width = canvas->width();
    int height = canvas->height();
    RGBA *data = canvas->data();

    int targetWidth = width * a;
    std::vector<RGBA> buffer(targetWidth * height);

    float constTerm = (1 - a) / (2 * a);

    float kRadius = 1;

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < targetWidth; c++) {
            
            float redAcc = 0.0f, greenAcc = 0.0f, blueAcc = 0.0f;

            float colTerm = (c / a) + constTerm;

            int iInitial = colTerm - kRadius;
            int iFinal = colTerm + kRadius;

            for (int i = iInitial; i < iFinal + 1; i++) {
                float x = colTerm - i;
                float coeff = tri(x);

                RGBA pixel = data[Helpers::index2D(r, adjustIndex(i, width), width)];
                redAcc += coeff * (pixel.r / 255.0f);
                greenAcc += coeff * (pixel.g / 255.0f);
                blueAcc += coeff * (pixel.b / 255.0f);
            }
            
            buffer[Helpers::index2D(r, c, targetWidth)] = RGBA(Helpers::realToByte(redAcc), Helpers::realToByte(greenAcc), Helpers::realToByte(blueAcc));

        }
    }

    canvas->resize(targetWidth, height);
    memcpy(canvas->data(), buffer.data(), sizeof(RGBA) * targetWidth * height);

}

void ScaleFilter::scaleUpHeight(Canvas2D *canvas, float a) {

    int width = canvas->width();
    int height = canvas->height();
    RGBA *data = canvas->data();

    int targetHeight = height * a;
    std::vector<RGBA> buffer(width * targetHeight);

    float constTerm = (1 - a) / (2 * a);

    float kRadius = 1;


    for (int r = 0; r < targetHeight; r++) {
        for (int c = 0; c < width; c++) {
            
            float redAcc = 0.0f, greenAcc = 0.0f, blueAcc = 0.0f;

            float rowTerm = (r / a) + constTerm;

            int iInitial = rowTerm - kRadius;
            int iFinal = rowTerm + kRadius;

            for (int i = iInitial; i < iFinal + 1; i++) {
                float x = rowTerm - i;
                float coeff = tri(x);

                RGBA pixel = data[Helpers::index2D(adjustIndex(i, height), c, width)];
                redAcc += coeff * (pixel.r / 255.0f);
                greenAcc += coeff * (pixel.g / 255.0f);
                blueAcc += coeff * (pixel.b / 255.0f);
            }
            
            buffer[Helpers::index2D(r, c, width)] = RGBA(Helpers::realToByte(redAcc), Helpers::realToByte(greenAcc), Helpers::realToByte(blueAcc));

        }
    }

    canvas->resize(width, targetHeight);
    memcpy(canvas->data(), buffer.data(), sizeof(RGBA) * width * targetHeight);

}


void ScaleFilter::scaleDownWidth(Canvas2D *canvas, float a) {
    
    int width = canvas->width();
    int height = canvas->height();
    RGBA *data = canvas->data();

    int targetWidth = glm::ceil(width * a);
    std::vector<RGBA> buffer(targetWidth * height);

    a = 1.0f / a;

    float kRadius = 1;

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < targetWidth; c++) {
            
            float redAcc = 0.0f, greenAcc = 0.0f, blueAcc = 0.0f;

            float colTerm = c + (a - 1) / (2 * a);

            int iInitial = a * (colTerm - kRadius);
            int iFinal = a * (colTerm + kRadius);

            float coeffSum = 0.0f;

            for (int i = iInitial; i < iFinal + 1; i++) {
                float x = colTerm - (i / a);
                float coeff = tri(x);

                if (i < 0 || i >= width) {
                    continue;
                }

                coeffSum += coeff;

                RGBA pixel = data[Helpers::index2D(r, i, width)];
                redAcc += coeff * (pixel.r / 255.0f);
                greenAcc += coeff * (pixel.g / 255.0f);
                blueAcc += coeff * (pixel.b / 255.0f);
            }
            
            buffer[Helpers::index2D(r, c, targetWidth)] = RGBA(Helpers::realToByte(redAcc / coeffSum), Helpers::realToByte(greenAcc / coeffSum), Helpers::realToByte(blueAcc / coeffSum));

        }
    }

    canvas->resize(targetWidth, height);
    memcpy(canvas->data(), buffer.data(), sizeof(RGBA) * targetWidth * height);

}


void ScaleFilter::scaleDownHeight(Canvas2D *canvas, float a) {

    int width = canvas->width();
    int height = canvas->height();
    RGBA *data = canvas->data();

    int targetHeight = glm::ceil(height * a);
    std::vector<RGBA> buffer(width * targetHeight);

    a = 1.0f / a;

    float constTerm = (a - 1) / (2 * a);

    float kRadius = 1;

    for (int r = 0; r < targetHeight; r++) {
        for (int c = 0; c < width; c++) {
            
            float redAcc = 0.0f, greenAcc = 0.0f, blueAcc = 0.0f;

            float rowTerm = r + constTerm;

            int iInitial = a * (rowTerm - kRadius);
            int iFinal = a * (rowTerm + kRadius);

            float sumCoeff = 0.0f;

            for (int i = iInitial; i < iFinal + 1; i++) {
                float x = rowTerm - (i / a);
                float coeff = tri(x) / a;
                sumCoeff += coeff;

                RGBA pixel = data[Helpers::index2D(adjustIndex(i, height), c, width)];
                redAcc += coeff * (pixel.r / 255.0f);
                greenAcc += coeff * (pixel.g / 255.0f);
                blueAcc += coeff * (pixel.b / 255.0f);
            }
            
            buffer[Helpers::index2D(r, c, width)] = RGBA(Helpers::realToByte(redAcc / sumCoeff), Helpers::realToByte(greenAcc / sumCoeff), Helpers::realToByte(blueAcc / sumCoeff));

        }
    }

    canvas->resize(width, targetHeight);
    memcpy(canvas->data(), buffer.data(), sizeof(RGBA) * width * targetHeight);

}






