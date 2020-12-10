#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"

#include <vector>


/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene {
public:
    RayScene(Scene &scene);
    virtual ~RayScene();

    std::vector<CS123ScenePrimitive> getPrimitives();
    std::vector<glm::mat4> getTransformations();
    std::vector<glm::mat4> getInverseTransformations();
    std::vector<CS123SceneLightData> getLights();
    CS123SceneGlobalData getGlobalData();
    std::vector<glm::mat3> getNormalTransformation();

    std::vector<glm::mat4> m_inverseTransformations;
    std::vector<glm::mat3> m_normalTransformations;

};

#endif // RAYSCENE_H
