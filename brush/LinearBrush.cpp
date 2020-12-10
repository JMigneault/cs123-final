/**
 * @file   LinearBrush.cpp
 *
 * Implementation of a brush with a linear mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "LinearBrush.h"

#include <math.h>

LinearBrush::LinearBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


LinearBrush::~LinearBrush()
{
}

void LinearBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Linear brush here...

    int r = Brush::getRadius();
    m_mask.resize((r + 1) * (r + 1));
    for (int i = 0; i < r + 1; i++) {
        for (int j = 0; j < r + 1; j++) {
            float p = (r - fmin(sqrt(i * i + j * j), r)) / r;
            m_mask[i * (r + 1) + j] = p;
        }

    }


}


