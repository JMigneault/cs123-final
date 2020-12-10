/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include "SmudgeBrush.h"

#include "Canvas2D.h"
#include "math.h"
#include "Brush.h"

SmudgeBrush::SmudgeBrush(RGBA color, int radius) :
    QuadraticBrush(color, radius)
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. Remember to use automatically managed memory!

    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!  If you're using automatic memory management everywhere,
    //       this will be empty.
}

void SmudgeBrush::brushUp(int x, int y, Canvas2D* canvas) {
    // prevents appearance of previously smudged colors in the edge case when you smudge the edge of the canvas
    m_paint_buffer.clear();
}


//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!
    //

    // Quadratic mask
    int r = Brush::getRadius();
    float A = 1.0 / (r * r);
    float B = -2.0 / r;
    float C = 1.0;

    m_mask.resize((r + 1) * (r + 1));
    for (int i = 0; i < r + 1; i++) {
        for (int j = 0; j < r + 1; j++) {
            float dist = sqrt(i * i + j * j);
            if (dist > r) {
                m_mask[i * (r + 1) + j] = 0;
            } else {
                m_mask[i * (r + 1) + j] = fmax(A * dist * dist + B * dist + C, 0);
            }
        }

    }

}

void SmudgeBrush::brushDown(int x, int y, Canvas2D *canvas) {
    pickUpPaint(x, y, canvas);
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas) {

    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //

    RGBA *data = canvas->data();
    int r = getRadius();

    m_paint_buffer.resize((2 * r + 1) * (2 * r + 1));

    int rowStart = qMax(y - r, 0);
    int rowEnd = qMin(y + r + 1, canvas->height());
    int colStart = qMax(x - r, 0);
    int colEnd = qMin(x + r + 1, canvas->width());

    m_buffer_row_offset = rowStart;
    m_buffer_col_offset = colStart;

    for (int row = rowStart; row < rowEnd; row++) {
        for (int col = colStart; col < colEnd; col++) {
            m_paint_buffer[(row - (y - r)) * (2 * r + 1) + col - (x - r)] = data[row * canvas->width() + col];
        }
    }
}

void SmudgeBrush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.

    RGBA *data = canvas->data();
    int r = getRadius();
    int rowStart = qMax(mouseY - r, 0);
    int rowEnd = qMin(mouseY + r + 1, canvas->height());
    int colStart = qMax(mouseX - r, 0);
    int colEnd = qMin(mouseX + r + 1, canvas->width());

    for (int row = rowStart; row < rowEnd; row++) {
        for (int col = colStart; col < colEnd; col++) {

            float m = m_mask[qAbs(mouseY - row) * (r + 1) + qAbs(mouseX - col)];

            RGBA b = m_paint_buffer[(row - rowStart) * (2 * r + 1) + col - colStart];
            // RGBA b = m_paint_buffer[(row - m_buffer_row_offset) * (2 * r + 1) + col - m_buffer_col_offset];
            RGBA c = data[row * canvas->width() + col];

            RGBA final = {0, 0, 0, 255};

            for (int i = 0; i < 3; i++) { // set r, g, and b
                // NOTE: ignoring alpha
                float a = 1.0;
                final.channels[i] = lerp(c.channels[i], b.channels[i], a * m);
            }

            data[row * canvas->width() + col] = final;
        }
    }

    // now pick up paint again...
    pickUpPaint(mouseX, mouseY, canvas);

}


