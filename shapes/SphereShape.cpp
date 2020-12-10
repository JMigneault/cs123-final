#include "SphereShape.h"

// input vector has form (r, phi, theta)
glm::vec3 spherToCart(glm::vec3 v) {
    return glm::vec3(v.x * glm::sin(v.y) * glm::cos(v.z),
                     v.x * glm::cos(v.y),
                     v.x * glm::sin(v.y) * glm::sin(v.z));

}

SphereShape::SphereShape()
{

}

SphereShape::SphereShape(int param1, int param2) :
    m_param1(glm::max(3, param1)), m_param2(glm::max(3, param2))
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

void SphereShape::buildVertices() {

    int stacks = m_param1;
    int slices = m_param2;

    float dTheta = TWOPI / slices;
    float dPhi = glm::pi<float>() / stacks;
    float radius = 0.5f;


    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {

            float iPhi = dPhi * i;
            float iTheta = dTheta * j;

            // triangle 1
            insertVec3(m_vertexData, spherToCart(glm::vec3(radius, iPhi, iTheta + dTheta)));
            insertVec3(m_vertexData, spherToCart(glm::vec3(1, iPhi, iTheta + dTheta)));

            insertVec3(m_vertexData, spherToCart(glm::vec3(radius, iPhi + dPhi, iTheta)));
            insertVec3(m_vertexData, spherToCart(glm::vec3(1, iPhi + dPhi, iTheta)));

            insertVec3(m_vertexData, spherToCart(glm::vec3(radius, iPhi, iTheta)));
            insertVec3(m_vertexData, spherToCart(glm::vec3(1, iPhi, iTheta)));


            // triangle 2
            insertVec3(m_vertexData, spherToCart(glm::vec3(radius, iPhi, iTheta + dTheta)));
            insertVec3(m_vertexData, spherToCart(glm::vec3(1, iPhi, iTheta + dTheta)));

            insertVec3(m_vertexData, spherToCart(glm::vec3(radius, iPhi + dPhi, iTheta + dTheta)));
            insertVec3(m_vertexData, spherToCart(glm::vec3(1, iPhi + dPhi, iTheta + dTheta)));

            insertVec3(m_vertexData, spherToCart(glm::vec3(radius, iPhi + dPhi, iTheta)));
            insertVec3(m_vertexData, spherToCart(glm::vec3(1, iPhi + dPhi, iTheta)));

        }
    }

}



