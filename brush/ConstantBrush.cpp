/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "ConstantBrush.h"
#include "Canvas2D.h"

ConstantBrush::ConstantBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
}

void ConstantBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Constant brush here...
    int r = Brush::getRadius();
    m_mask.resize((r + 1) * (r + 1));
    for (int i = 0; i < r + 1; i++) {
        for (int j = 0; j < r + 1; j++) {
            if (i * i + j * j <= (r + 1) * (r + 1)) {
                m_mask[i * (r + 1) + j] = 1;
            } else {
                m_mask[i * (r + 1) + j] = 0;
            }
        }

    }


}


