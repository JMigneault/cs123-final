#include "FinalScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"

#include "Settings.h"
#include "SupportCanvas3D.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
#include "CS123SceneData.h"
#include "gl/datatype/FBO.h"
#include "openglshape.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include "gl/textures/Texture2D.h"

using namespace CS123::GL;


FinalScene::FinalScene(int width, int height):
    m_cube(new CubeShape(settings.shapeParameter1)),
    m_cone(new ConeShape(settings.shapeParameter1, settings.shapeParameter2)),
    m_cylinder(new CylinderShape(settings.shapeParameter1, settings.shapeParameter2)),
    m_sphere(new SphereShape(settings.shapeParameter1, settings.shapeParameter2)),
    m_height(height), m_width(width), m_occlFBO(nullptr), m_lightFBO(nullptr), m_phongFBO(nullptr), m_crepFBO(nullptr)
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();

    loadOcclShader();
    loadLightShader();
    loadCrepShader();
    loadMergeShader();

    initializeFBOs();
    buildQuad();

}

FinalScene::~FinalScene()
{
}

void FinalScene::buildQuad() {
    // Make fullscreen quad

    std::vector<GLfloat> quadData = {-1.0f, 1.0f, 0, 0,
                                      -1.0f, -1.0f, 0, 1,
                                      1.0,   1.0f, 1, 0,
                                      1.0f, -1.0f, 1, 1};

    m_quad = std::make_unique<OpenGLShape>();
    m_quad->setVertexData(&quadData[0], quadData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
    m_quad->setAttribute(ShaderAttrib::POSITION, 2, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->setAttribute(ShaderAttrib::TEXCOORD0, 2, 2*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->buildVAO();

}

void FinalScene::initializeFBOs() {
    // Make FBOs
    m_phongFBO = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE,
                                      m_width, m_height,
                                      TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                      TextureParameters::FILTER_METHOD::NEAREST, GL_FLOAT);

    m_occlFBO = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE,
                                      m_width, m_height,
                                      TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                      TextureParameters::FILTER_METHOD::NEAREST, GL_FLOAT);

    m_lightFBO = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE,
                                      m_width, m_height,
                                      TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                      TextureParameters::FILTER_METHOD::NEAREST, GL_FLOAT);


    m_crepFBO = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE,
                                      m_width, m_height,
                                      TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                      TextureParameters::FILTER_METHOD::NEAREST, GL_FLOAT);
}

void FinalScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void FinalScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void FinalScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void FinalScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

// Cresp
void FinalScene::loadOcclShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/occl.frag");
    m_occlShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void FinalScene::loadLightShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/quad.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/light.frag");
    m_lightShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void FinalScene::loadCrepShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/quad.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/crep.frag");
    m_crepShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void FinalScene::loadMergeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/quad.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/merge.frag");
    m_mergeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void FinalScene::render(SupportCanvas3D *context) {
    printf("RENDERING!");
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 sunPosition = glm::vec3(0.0, 0.0, 100000.0);
    glm::vec4 screenSunPosUnnorm = (context->getCamera()->getProjectionMatrix()
                              * context->getCamera()->getViewMatrix()
                              * glm::vec4(sunPosition, 1));
    glm::vec2 screenSunPos = screenSunPosUnnorm.xy() / screenSunPosUnnorm.w;

    // normal phong render
    m_phongFBO->bind();
    m_phongShader->bind();
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_width, m_height);
    setSceneUniforms(context, m_phongShader);
    setLights(m_phongShader);
    renderGeometry(m_phongShader);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();
    m_phongFBO->unbind();

    // render occluding geometry
    m_occlFBO->bind();
    m_occlShader->bind();
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_width, m_height);
    setSceneUniforms(context, m_occlShader);
    setLights(m_occlShader);
    renderGeometry(m_occlShader);
    m_occlShader->unbind();
    m_occlFBO->unbind();

    // render lighting
    m_lightFBO->bind();
    m_lightShader->bind();

    glActiveTexture(GL_TEXTURE0); // TODO: necessary??
    m_occlFBO->getColorAttachment(0).bind();

    GLint texPosition = glGetUniformLocation(m_lightShader->getID(), "tex");
    glUniform1i(texPosition, 0);

    // constants
    glm::vec3 color(0.05f, 0.05f, 0.12f);
    float sunStrength = 1.0f;
    float sunDecay = 1.0f;

    // Uniforms
    m_lightShader->setUniform("color", color);
    m_lightShader->setUniform("screenSunPos", screenSunPos);
    m_lightShader->setUniform("sunStrength", sunStrength);
    m_lightShader->setUniform("sunDecay", sunDecay);

    m_quad->draw();
    m_lightShader->unbind();
    m_lightFBO->unbind();
    m_occlFBO->getColorAttachment(0).unbind();

    // render crepescular rays
    m_crepFBO->bind();
    m_crepShader->bind();
    m_lightFBO->getColorAttachment(0).bind();

    // constants
    float exposure = 0.95f;
    float decay = .995f;
    float weight = 0.9;
    float density = 1.0;

    // Uniforms
    m_crepShader->setUniform("screenSunPos", screenSunPos);
    m_crepShader->setUniform("exposure", exposure);
    m_crepShader->setUniform("decay", decay);
    m_crepShader->setUniform("weight", weight);
    m_crepShader->setUniform("density", density);

    m_quad->draw();
    m_crepShader->unbind();
    m_crepFBO->unbind();
    m_lightFBO->getColorAttachment(0).unbind();

    // merge crepescular rays and phong outputs
    m_mergeShader->bind();

    GLint crepescularLocation = glGetUniformLocation(m_mergeShader->getID(), "crepescular");
    glUniform1i(crepescularLocation, 0);
    GLint sceneLocation = glGetUniformLocation(m_mergeShader->getID(), "scene");
    glUniform1i(sceneLocation, 1);

    glActiveTexture(GL_TEXTURE0);
    m_crepFBO->getColorAttachment(0).bind();

    glActiveTexture(GL_TEXTURE1);
    m_phongFBO->getColorAttachment(0).bind();

    glActiveTexture(GL_TEXTURE0);

    // constants
    float phongWeight = 1.0f;

    m_mergeShader->setUniform("phongWeight", phongWeight);

    m_quad->draw();

    m_mergeShader->unbind();

    m_crepFBO->getColorAttachment(0).unbind();
    m_phongFBO->getColorAttachment(0).unbind();

}

void FinalScene::setSceneUniforms(SupportCanvas3D *context, std::unique_ptr<CS123Shader> &shader) {
    Camera *camera = context->getCamera();
    shader->setUniform("useLighting", settings.useLighting);
    shader->setUniform("useArrowOffsets", false);
    shader->setUniform("p" , camera->getProjectionMatrix());
    shader->setUniform("v", camera->getViewMatrix());

    // TODO: add new informs
}

void FinalScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
}

void FinalScene::setLights(std::unique_ptr<CS123Shader> &shader)
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

void FinalScene::renderGeometry(std::unique_ptr<CS123Shader> &shader) {
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

        // m_phongShader->setUniform("m", trans);
        shader->setUniform("m", trans);
        primitive.material.cDiffuse *= m_global.kd;
        primitive.material.cAmbient *= m_global.ka;

        shader->applyMaterial(primitive.material);

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

void FinalScene::settingsChanged() {
    // TODO: [SCENEVIEW] Fill this in if applicable.
    m_cube = std::make_unique<CubeShape>(settings.shapeParameter1);
    m_cone = std::make_unique<ConeShape>(settings.shapeParameter1, settings.shapeParameter2);
    m_cylinder = std::make_unique<CylinderShape>(settings.shapeParameter1, settings.shapeParameter2);
    m_sphere = std::make_unique<SphereShape>(settings.shapeParameter1, settings.shapeParameter2);

}

