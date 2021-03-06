#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"

#include "Settings.h"
#include "SupportCanvas3D.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
#include "CS123SceneData.h"
using namespace CS123::GL;


SceneviewScene::SceneviewScene():
    m_cube(new CubeShape(settings.shapeParameter1)),
    m_cone(new ConeShape(settings.shapeParameter1, settings.shapeParameter2)),
    m_cylinder(new CylinderShape(settings.shapeParameter1, settings.shapeParameter2)),
    m_sphere(new SphereShape(settings.shapeParameter1, settings.shapeParameter2))
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();

}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::render(SupportCanvas3D *context) {
    setClearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->bind();
    setSceneUniforms(context);
    setLights();
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();

}

void SceneviewScene::setSceneUniforms(SupportCanvas3D *context) {
    Camera *camera = context->getCamera();
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("useArrowOffsets", false);
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());
}

void SceneviewScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
}

void SceneviewScene::setLights()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Set up the lighting for your scene using m_phongShader.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //

    for (int i = 0; i < m_lights.size(); i++) {
        m_phongShader->setLight(m_lights[i]);
    }

}

void SceneviewScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // TODO: [SCENEVIEW] Fill this in...
    // You shouldn't need to write *any* OpenGL in this class!
    //
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //

    for (int i = 0; i < m_primitives.size(); i++) {
        CS123ScenePrimitive primitive = m_primitives[i];
        glm::mat4 trans = m_transformations[i];

        m_phongShader->setUniform("m", trans);
        primitive.material.cDiffuse *= m_global.kd;
        primitive.material.cAmbient *= m_global.ka;

        m_phongShader->applyMaterial(primitive.material);

        switch (primitive.type) {
            case PrimitiveType::PRIMITIVE_CUBE:
                m_cube->draw();
                break;
            case PrimitiveType::PRIMITIVE_CONE:
                m_cone->draw();
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                m_cylinder->draw();

                break;
            case PrimitiveType::PRIMITIVE_SPHERE:
                m_sphere->draw();
                break;
        }
    }

}

void SceneviewScene::settingsChanged() {
    // TODO: [SCENEVIEW] Fill this in if applicable.
    m_cube = std::make_unique<CubeShape>(settings.shapeParameter1);
    m_cone = std::make_unique<ConeShape>(settings.shapeParameter1, settings.shapeParameter2);
    m_cylinder = std::make_unique<CylinderShape>(settings.shapeParameter1, settings.shapeParameter2);
    m_sphere = std::make_unique<SphereShape>(settings.shapeParameter1, settings.shapeParameter2);

}

