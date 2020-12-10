#ifndef RECTANGULARSIDE_H
#define RECTANGULARSIDE_H

#include "CircularSide.h"

class RectangularSide: public ShapeComponent
{
public:
    RectangularSide();
    RectangularSide(int rows, int cols, float height, float width);
    void buildSide() override;

    int m_rows;
    int m_cols;
    float m_width;
    float m_height;
};

#endif // RECTANGULARSIDE_H
