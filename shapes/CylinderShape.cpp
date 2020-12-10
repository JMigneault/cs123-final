#include "CylinderShape.h"

glm::vec3 inline xzToXyz(glm::vec2 xz, float y)  {
    return glm::vec3(xz.y, y, xz.x);
}

CylinderShape::CylinderShape()
{

}

CylinderShape::CylinderShape(int param1, int param2) :
    m_param1(param1), m_param2(glm::max(3, param2))
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

void CylinderShape::buildVertices() {

    std::unique_ptr<CircularSide> circularSide1(new CircularSide(m_param1, m_param2));
    circularSide1->transformSide(glm::translate(glm::vec3(0, -.5, 0)), glm::rotate(glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0)) * glm::rotate(glm::pi<float>() / 2.0f, glm::vec3(0, 0, 1)));
    circularSide1->ShapeComponent::drawSide(m_vertexData);

    std::unique_ptr<CircularSide> circularSide2(new CircularSide(m_param1, m_param2));
    circularSide2->transformSide(glm::translate(glm::vec3(0, .5, 0)), glm::rotate(-glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0)) * glm::rotate(- glm::pi<float>() / 2.0f, glm::vec3(0, 0, 1)));
    circularSide2->ShapeComponent::drawSide(m_vertexData);

    int stacks = m_param1;
    int slices = m_param2;

    float radius = 0.5f;
    float theta = TWOPI / slices;

    float sideWidth = 2 * radius * glm::sin(theta / 2.0f);
    float sideHeight = 1.0f / stacks;

    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            float angi = theta * j;
            float yi = i * sideHeight - 0.5f;
            // triangle 1
            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(radius, angi)), yi));
            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(1.0f, angi)), 0));

            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(radius, angi + theta)), yi));
            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(1.0f, angi + theta)), 0));

            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(radius, angi + theta)), yi + sideHeight));
            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(1.0f, angi + theta)), 0));


            // triangle 2
            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(radius, angi)), yi));
            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(1.0f, angi)), 0));

            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(radius, angi + theta)), yi + sideHeight));
            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(1.0f, angi + theta)), 0));

            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(radius, angi)), yi + sideHeight));
            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(1.0f, angi)), 0));

        }
    }

}

