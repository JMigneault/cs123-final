#ifndef SPHERESHAPE_H
#define SPHERESHAPE_H

#include "Shape.h"


class SphereShape : public Shape
{
public:
    SphereShape();
    SphereShape(int param1, int param2);

    void buildVertices();

    int m_param1;
    int m_param2;
};

#endif // SPHERESHAPE_H
