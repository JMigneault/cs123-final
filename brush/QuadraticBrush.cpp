/**
 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

#include <math.h>

QuadraticBrush::QuadraticBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
}

void QuadraticBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Quadratic brush here...

    int r = Brush::getRadius();
    float A = 1.0 / (r * r);
    float B = -2.0 / r;
    float C = 1.0;

    m_mask.resize((r + 1) * (r + 1));
    for (int i = 0; i < r + 1; i++) {
        for (int j = 0; j < r + 1; j++) {
            float dist = sqrt(i * i + j * j);
            // no paint outside the brush radius
            if (dist > r) {
                m_mask[i * (r + 1) + j] = 0;
            } else {
                m_mask[i * (r + 1) + j] = fmax(A * dist * dist + B * dist + C, 0);
            }
        }

    }

}


