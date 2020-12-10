#ifndef CYLINDERSHAPE_H
#define CYLINDERSHAPE_H

#include "Shape.h"
#include "RectangularSide.h"

class CylinderShape : public Shape
{
public:
    CylinderShape();
    CylinderShape(int param1, int param2);

    void buildVertices();

    int m_param1;
    int m_param2;

};

#endif // CYLINDERSHAPE_H
