#ifndef SHAPE_H
#define SHAPE_H

/** imports the OpenGL math library https://glm.g-truc.net/0.9.2/api/a00001.html */
#include <glm/glm.hpp>
#include "GL/glew.h"

/** used to transform vertices for shape building */
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/constants.hpp"

#include<memory>
#include <vector>


// constants
const float TWOPI = 2 * glm::pi<float>();

/**
 *
 * inserts a glm::vec3 into a vector of floats
 * this will come in handy if you want to take advantage of vectors to build your shape
 * make sure to call reserve beforehand to speed this up
 */
void insertVec3(std::vector<float> &data, glm::vec3 v);

namespace CS123 { namespace GL {
class VAO;
}}

class Shape
{
public:
    Shape();
    ~Shape();
    void draw();

protected:
    /** builds the VAO, pretty much the same as from lab 1 */
    void buildVAO();

    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;
};

#endif // SHAPE_H
