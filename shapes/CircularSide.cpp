#include "CircularSide.h"

CircularSide::CircularSide()
{

}

CircularSide::CircularSide(int circles, int slices) : m_circles(circles), m_slices(slices) {
    buildSide();
}


void CircularSide::buildSide() {

    float rSep = 0.5f / m_circles;
    float thSep = TWOPI / m_slices;

    // for each concentric circle
    for (int i = 0; i < m_circles; i++) {
        // for each slice of the circle
        for (int j = 0; j < m_slices; j++) {

            float startR = i * rSep;
            float startTh = j * thSep;

            m_side.push_back(
                        glm::vec3(polToCart(glm::vec2(startR, startTh)), 0));
            m_side.push_back(
                        glm::vec3(polToCart(glm::vec2(startR + rSep, startTh)), 0));
            m_side.push_back(
                        glm::vec3(polToCart(glm::vec2(startR, startTh + thSep)), 0));
            m_side.push_back(
                        glm::vec3(polToCart(glm::vec2(startR + rSep, startTh)), 0));
            m_side.push_back(
                        glm::vec3(polToCart(glm::vec2(startR + rSep, startTh + thSep)), 0));
            m_side.push_back(
                        glm::vec3(polToCart(glm::vec2(startR, startTh + thSep)), 0));


        }
    }

}
