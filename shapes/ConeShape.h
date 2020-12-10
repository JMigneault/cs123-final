#ifndef CONESHAPE_H
#define CONESHAPE_H

#include "Shape.h"
#include "CircularSide.h"

class ConeShape : public Shape
{
public:
    ConeShape();
    ConeShape(int param1, int param2);

    void buildVertices();

    int m_param1;
    int m_param2;
};

#endif // CONESHAPE_H
