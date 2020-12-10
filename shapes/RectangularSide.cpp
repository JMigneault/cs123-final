#include "RectangularSide.h"

RectangularSide::RectangularSide(int rows, int cols, float height, float width) :
    m_rows(rows), m_cols(cols), m_width(width), m_height(height)
{
    buildSide();
}

void RectangularSide::buildSide() {
    for (int i = 0; i < m_cols; i++) {
        for (int j = 0; j < m_rows; j++) {
            float xi = i * m_width - (m_width * m_cols / 2.0f);
            float yi = j * m_height - (m_height * m_rows / 2.0f);

            // tri1
            m_side.push_back(glm::vec3(xi, yi, 0));
            m_side.push_back(glm::vec3(xi + m_width, yi + m_height, 0));
            m_side.push_back(glm::vec3(xi, yi + m_height, 0));

            // tri2
            m_side.push_back(glm::vec3(xi, yi, 0));
            m_side.push_back(glm::vec3(xi + m_width, yi, 0));
            m_side.push_back(glm::vec3(xi + m_width, yi + m_height, 0));
        }
    }

}

