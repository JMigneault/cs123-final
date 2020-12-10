#include "ShapeComponent.h"

ShapeComponent::ShapeComponent() : m_side(std::vector<glm::vec3>()), m_norms(glm::vec3(0, 0, 1))
{

}

void ShapeComponent::transformSide(glm::mat4 translation, glm::mat4 rotation) {
    glm::mat4 transformation = translation * rotation;
    for (int i = 0; i < m_side.size(); i++) {
        glm::vec4 v = glm::vec4(m_side[i], 1);
        m_side[i] = (transformation * v).xyz();
    }

    glm::vec4 v = glm::vec4(m_norms, 1);
    m_norms = (rotation * v).xyz();

}

void ShapeComponent::drawSide(std::vector<float> &m_vertexData) {
    for (int i = 0; i < m_side.size(); i++) {
        insertVec3(m_vertexData, m_side[i]);
        insertVec3(m_vertexData, m_norms);
    }

}

