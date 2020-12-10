#ifndef CIRCULARSIDE_H
#define CIRCULARSIDE_H

#include "ShapeComponent.h"


class CircularSide : public ShapeComponent
{
public:
    CircularSide();
    CircularSide(int circles, int slices);
    void buildSide() override;

    int m_circles;
    int m_slices;

};

#endif // CIRCULARSIDE_H
