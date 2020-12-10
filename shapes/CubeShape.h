#ifndef CUBESHAPE_H
#define CUBESHAPE_H

#include "Shape.h"
#include "RectangularSide.h"

class CubeShape : public Shape
{
public:
    CubeShape();
    CubeShape(int param1);
    void buildVertices();

    int m_param1;

};

#endif // CUBESHAPE_H
