#include "ConeShape.h"

// helper function to convert from 2d on the xy plane to 3d on the zx plane with given y value
glm::vec3 inline xzToXyz(glm::vec2 xz, float y)  {
    return glm::vec3(xz.y, y, xz.x);
}

ConeShape::ConeShape()
{

}

ConeShape::ConeShape(int param1, int param2) :
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

void ConeShape::buildVertices() {

    std::unique_ptr<CircularSide> circularSide1(new CircularSide(m_param1, m_param2));
    circularSide1->transformSide(glm::translate(glm::vec3(0, -.5, 0)), glm::rotate(glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0)) * glm::rotate(glm::pi<float>() / 2.0f, glm::vec3(0, 0, 1)));
    circularSide1->ShapeComponent::drawSide(m_vertexData);

    int stacks = m_param1;
    int slices = m_param2;

    float theta = TWOPI / slices;

    float sideHeight = 1.0f / stacks;
    float dR = 0.5f / stacks;
    glm::vec2 xyNorm;

    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            float radius = dR * (stacks - i);
            float angi = theta * j;
            float yi = i * sideHeight - 0.5f;
            // triangle 1
            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(radius, angi)), yi));
            xyNorm = polToCart(glm::vec2(1.0f, angi));
            insertVec3(m_vertexData, glm::normalize(xzToXyz(xyNorm, 0.5f)));

            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(radius, angi + theta)), yi));
            xyNorm = polToCart(glm::vec2(1.0f, angi + theta));
            insertVec3(m_vertexData, glm::normalize(xzToXyz(xyNorm, 0.5f)));

            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(radius - dR, angi + theta)), yi + sideHeight));
            xyNorm = polToCart(glm::vec2(1.0f, angi + theta));
            insertVec3(m_vertexData, glm::normalize(xzToXyz(xyNorm, 0.5f)));

            // triangle 2
            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(radius, angi)), yi));
            xyNorm = polToCart(glm::vec2(1.0f, angi));
            insertVec3(m_vertexData, glm::normalize(xzToXyz(xyNorm, 0.5f)));

            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(radius - dR, angi + theta)), yi + sideHeight));
            xyNorm = polToCart(glm::vec2(1.0f, angi + theta));
            insertVec3(m_vertexData, glm::normalize(xzToXyz(xyNorm, 0.5f)));

            insertVec3(m_vertexData, xzToXyz(polToCart(glm::vec2(radius - dR, angi)), yi + sideHeight));
            xyNorm = polToCart(glm::vec2(1.0f, angi));
            insertVec3(m_vertexData, glm::normalize(xzToXyz(xyNorm, 0.5f)));

        }
    }

}

