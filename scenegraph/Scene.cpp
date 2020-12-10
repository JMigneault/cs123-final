#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"

#include "glm/gtx/transform.hpp"


Scene::Scene()
{
}

Scene::Scene(Scene &scene)
{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    CS123SceneGlobalData global = { 1, 1, 1, 1};
    setGlobal(global);

    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.
    this->m_lights = scene.m_lights;
    this->m_primitives = scene.m_primitives;
    this->m_transformations = scene.m_transformations;
    setGlobal(scene.m_global);

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

glm::mat4 composeTransformations(glm::mat4 &result, std::vector<CS123SceneTransformation *> transformations) {

    for (int i = 0; i < transformations.size(); i++) {
        CS123SceneTransformation *trans = transformations[i];
        switch(trans->type) {
            case TRANSFORMATION_TRANSLATE:
                result = result * glm::translate(trans->translate);
                break;
            case TRANSFORMATION_SCALE:
                result = result * glm::scale(trans->scale);
                break;
            case TRANSFORMATION_ROTATE:
                result = result * glm::rotate(trans->angle, trans->rotate);
                break;
            case TRANSFORMATION_MATRIX:
                result = result * trans->matrix;
                break;
        }
    }

}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()

    // set camera done in canvas - not touched here


    // set globals
    CS123SceneGlobalData globalData;
    parser->getGlobalData(globalData);

    sceneToFill->setGlobal(globalData);

    // set lights
    int numLights = parser->getNumLights();
    for (int i = 0; i < numLights; i++) {
        CS123SceneLightData lightData;
        parser->getLightData(i, lightData);
        sceneToFill->addLight(lightData);
    }

    // primitives
    std::vector<CS123SceneNode *> nodeStack;
    std::vector<glm::mat4> transformationsStack;
    std::vector<bool> visitedStack;

    CS123SceneNode *root = parser->getRootNode();

    glm::mat4 trans = glm::mat4();
    composeTransformations(trans, root->transformations);

    transformationsStack.push_back(trans);
    nodeStack.push_back(root);
    visitedStack.push_back(false);

    while (nodeStack.size() > 0) {
        CS123SceneNode next = *nodeStack.back();

        if (visitedStack.back()) {
            // we've visited all of this nodes children already
            nodeStack.pop_back();
            transformationsStack.pop_back();
            visitedStack.pop_back();
        } else {
            // set visited to true, add these primitives to the stack, and add children to the stack
            visitedStack.back() = true;

            trans = glm::mat4();
            composeTransformations(trans, next.transformations);
            transformationsStack.push_back(transformationsStack.back() * trans);

            for (int i = 0; i < next.primitives.size(); i++) {
                sceneToFill->addPrimitive(*next.primitives[i], transformationsStack.back());
            }

            for (int i = 0; i < next.children.size(); i++) {
                nodeStack.push_back(next.children[i]);
                visitedStack.push_back(false);
            }

        }

    }

}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
    m_primitives.push_back(scenePrimitive);
    m_transformations.push_back(matrix);
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    m_lights.push_back(sceneLight);
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_global = global;
}

