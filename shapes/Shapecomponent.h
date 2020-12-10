#ifndef SHAPECOMPONENT_H
#define SHAPECOMPONENT_H

#include <glm/glm.hpp>
#include "GL/glew.h"

/** used to transform vertices for shape building */
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/constants.hpp"

#include<memory>
#include <vector>

#include "Shape.h"


class ShapeComponent
{
public:
    ShapeComponent();
    void transformSide(glm::mat4 translation, glm::mat4 rotation);
    void drawSide(std::vector<float> &m_vertexData);
    virtual void buildSide() = 0;

    std::vector<glm::vec3> m_side;
    glm::vec3 m_norms;

};

// convert from polar to cartesian coordinates
glm::vec2 inline polToCart(glm::vec2 polV) {
    float r = polV[0];
    float th = polV[1];
    return glm::vec2(r * glm::cos(th), r * glm::sin(th));
}


#endif // SHAPECOMPONENT_H
