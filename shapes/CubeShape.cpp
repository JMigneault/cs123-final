#include "CubeShape.h"

CubeShape::CubeShape()
{

}
CubeShape::CubeShape(int param1) :
    m_param1(param1)
{
    /**
     * We build in vertex data for a cube, in this case they are handwritten.
     * You'll want to expand upon or refactor this code heavily to take advantage
     * of polymorphism, vector math/glm library, and utilize good software design
     *
     */
    buildVertices();
    /** build the VAO so that the shape is ready to be drawn */
    buildVAO();
}

void CubeShape::buildVertices() {

    float gridWidth = 1.0f / m_param1;

    std::unique_ptr<RectangularSide> rectangularSide1(new RectangularSide(m_param1, m_param1, gridWidth, gridWidth));
    rectangularSide1->transformSide(glm::translate(glm::vec3(0, 0, .5)), glm::rotate(0.0f, glm::vec3(1, 0, 0)));
    rectangularSide1->ShapeComponent::drawSide(m_vertexData);

    std::unique_ptr<RectangularSide> rectangularSide2(new RectangularSide(m_param1, m_param1, gridWidth, gridWidth));
    rectangularSide2->transformSide(glm::translate(glm::vec3(0, 0, -.5)), glm::rotate(glm::pi<float>(), glm::vec3(1, 0, 0)));
    rectangularSide2->drawSide(m_vertexData);

    std::unique_ptr<RectangularSide> rectangularSide3(new RectangularSide(m_param1, m_param1, gridWidth, gridWidth));
    rectangularSide3->transformSide(glm::translate(glm::vec3(.5, 0, 0)), glm::rotate(glm::pi<float>() / 2.0f, glm::vec3(0, 1, 0)));
    rectangularSide3->drawSide(m_vertexData);

    std::unique_ptr<RectangularSide> rectangularSide4(new RectangularSide(m_param1, m_param1, gridWidth, gridWidth));
    rectangularSide4->transformSide(glm::translate(glm::vec3(-.5, 0, 0)), glm::rotate(glm::pi<float>() / -2.0f, glm::vec3(0, 1, 0)));
    rectangularSide4->drawSide(m_vertexData);

    std::unique_ptr<RectangularSide> rectangularSide5(new RectangularSide(m_param1, m_param1, gridWidth, gridWidth));
    rectangularSide5->transformSide(glm::translate(glm::vec3(0, .5, 0)), glm::rotate(glm::pi<float>() / -2.0f, glm::vec3(1, 0, 0)));
    rectangularSide5->drawSide(m_vertexData);

    std::unique_ptr<RectangularSide> rectangularSide6(new RectangularSide(m_param1, m_param1, gridWidth, gridWidth));
    rectangularSide6->transformSide(glm::translate(glm::vec3(0, -.5, 0)), glm::rotate(glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0)));
    rectangularSide6->drawSide(m_vertexData);

}

