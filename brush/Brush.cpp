/**
 * @file   Brush.cpp
 *
 * Implementation of common functionality of bitmap brushes.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "Brush.h"
#include "Canvas2D.h"

Brush::Brush(RGBA color, int radius) :
    // Pro-tip: Initialize all variables in the initialization list
    m_color(color),
    m_radius(radius)
{
    // Pro-tip: By the time you get to the constructor body, all of the
    // member variables have already been initialized.
}


Brush::~Brush()
{
    // Pro-tip: The destructor (here) is where you free all the resources
    // you allocated during the lifetime of the class
    // Pro-tip: If you use std::unique_ptr or std::vector, you probabably
    // won't need to put anything in here.
}

int Brush::getAlpha() const {
    return m_color.a;
}

RGBA Brush::getRGBA() const {
    return m_color;
}

int Brush::getBlue() const {
    return m_color.b;
}

int Brush::getGreen() const {
    return m_color.g;
}

int Brush::getRadius() const {
    return m_radius;
}

int Brush::getRed() const {
    return m_color.r;
}

void Brush::setAlpha(int alpha) {
    m_color.a = alpha;
}

void Brush::setRGBA(const RGBA &rgba) {
    m_color = rgba;
}

void Brush::setBlue(int blue) {
    m_color.b = blue;
}

void Brush::setGreen(int green) {
    m_color.g = green;
}

void Brush::setRed(int red) {
    m_color.r = red;
}

void Brush::setRadius(int radius) {
    m_radius = radius;
    makeMask();
}




void Brush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] You can do any painting on the canvas here. Or, you can
    //        override this method in a subclass and do the painting there.
    //
    // Example: You'll want to delete or comment out this code, which
    // sets all the pixels on the canvas to red.
    //

    RGBA *data = canvas->data();
    int r = getRadius();
    int rowStart = qMax(mouseY - r, 0);
    int rowEnd = qMin(mouseY + r + 1, canvas->height());
    int colStart = qMax(mouseX - r, 0);
    int colEnd = qMin(mouseX + r + 1, canvas->width());

    for (int row = rowStart; row < rowEnd; row++) {
        for (int col = colStart; col < colEnd; col++) {

            float m = m_mask[qAbs(mouseY - row) * (r + 1) + qAbs(mouseX - col)];

            RGBA b = getRGBA();
            RGBA c = data[row * canvas->width() + col];

            RGBA final = {0, 0, 0, 255};
            for (int i = 0; i < 3; i++) { // set r, g, and b
                float a = b.a / 255.0;
                final.channels[i] = lerp(c.channels[i], b.channels[i], a * m);
            }

            data[row * canvas->width() + col] = final;
        }
    }


    /** EXAMPLE CODE
    RGBA *pix = canvas->data();
    int size = canvas->width() * canvas->height();
    for (int i = 0; i < size; i++) {
        pix[i] = RGBA(255, 0, 0, 255);
    }
    canvas->update();
    */

}
