#include "RayScene.h"
#include "Settings.h"
#include "CS123SceneData.h"

#include <iostream>


RayScene::RayScene(Scene &scene) :
    Scene(scene)
{
    // TODO [INTERSECT]
    // Remember that any pointers or OpenGL objects (e.g. texture IDs) will
    // be deleted when the old scene is deleted (assuming you are managing
    // all your memory properly to prevent memory leaks).  As a result, you
    // may need to re-allocate some things here.

    for (int i = 0; i < m_transformations.size(); i++) {
        m_inverseTransformations.push_back(glm::inverse(m_transformations[i]));
        m_normalTransformations.push_back(glm::inverse(glm::transpose(glm::mat3(m_transformations[i]))));
    }

}

RayScene::~RayScene()
{
}

std::vector<CS123ScenePrimitive> RayScene::getPrimitives() {
    return m_primitives;
}

std::vector<glm::mat4> RayScene::getTransformations() {
    return m_transformations;
}

std::vector<glm::mat4> RayScene::getInverseTransformations() {
    return m_inverseTransformations;
}

CS123SceneGlobalData RayScene::getGlobalData() {
    return m_global;
}

std::vector<CS123SceneLightData> RayScene::getLights() {
    return m_lights;
}

std::vector<glm::mat3> RayScene::getNormalTransformation() {
    return m_normalTransformations;
}
