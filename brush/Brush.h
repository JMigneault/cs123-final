#ifndef __BRUSH_H__    //header guards
#define __BRUSH_H__

#include <vector>

#include "RGBA.h"

// Suggested addition in piazza post @151
static unsigned char lerp(unsigned char a, unsigned char b, float percent) {
    float fa = static_cast<float>(a) / 255.0f;
    float fb = static_cast<float>(b) / 255.0f;
    return static_cast<unsigned char>((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}

class Canvas2D;

/**
 * @class Brush
 *
 * The superclass for all brushes!
 */

class Brush {
public:
    // Constructor for brush.  Pass it the initial color, flow and radius.
    Brush(RGBA color, int radius);

    // Destructor for brush.  Clean up all brush allocated resources.
    virtual ~Brush();

    RGBA getRGBA() const;
    int getAlpha() const;
    int getBlue() const;
    int getRed() const;
    int getGreen() const;
    int getRadius() const;

    void setAlpha(int alpha);
    void setBlue(int blue);
    void setGreen(int green);
    void setRed(int red);
    void setRGBA(const RGBA &rgba);
    void setRadius(int radius);

    virtual void brushDown(int x, int y, Canvas2D* canvas) = 0;
    virtual void brushDragged(int x, int y, Canvas2D* canvas);
    virtual void brushUp(int x, int y, Canvas2D* canvas) = 0;


protected:
    // Pure virtual function that will create the mask distribution.
    // To be be implemented in subclasses.
    virtual void makeMask() = 0;

    // C++ coding convention is to put m_ in front of member variables
    RGBA m_color;
    std::vector<float> m_mask;
    int m_radius;

};


#endif // __BRUSH_H__
