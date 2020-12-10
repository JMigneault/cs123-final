#ifndef FINALSCENE_H
#define FINALSCENE_H

#include "OpenGLScene.h"
#include "shapes/CubeShape.h"
#include "shapes/ConeShape.h"
#include "shapes/CylinderShape.h"
#include "shapes/SphereShape.h"
#include "gl/datatype/FBO.h"
#include "openglshape.h"

#include <memory>


namespace CS123 { namespace GL {

    class Shader;
    class CS123Shader;
    class Texture2D;
}}

/**
 *
 * @class SceneviewScene
 *
 * A complex scene consisting of multiple objects. Students will implement this class in the
 * Sceneview assignment.
 *
 * Here you will implement your scene graph. The structure is up to you - feel free to create new
 * classes and data structures as you see fit. We are providing this SceneviewScene class for you
 * to use as a stencil if you like.
 *
 * Keep in mind that you'll also be rendering entire scenes in the next two assignments, Intersect
 * and Ray. The difference between this assignment and those that follow is here, we are using
 * OpenGL to do the rendering. In Intersect and Ray, you will be responsible for that.
 */
class FinalScene : public OpenGLScene {
public:
    FinalScene(int width, int height);
    virtual ~FinalScene();

    virtual void render(SupportCanvas3D *context) override;
    virtual void settingsChanged() override;

    // Use this method to set an internal selection, based on the (x, y) position of the mouse
    // pointer.  This will be used during the "modeler" lab, so don't worry about it for now.
    void setSelection(int x, int y);

private:


private:

    void loadPhongShader();
    void loadWireframeShader();
    void loadNormalsShader();
    void loadNormalsArrowShader();

    void loadOcclShader();
    void loadLightShader();
    void loadCrepShader();
    void loadMergeShader();

    void buildQuad();
    void initializeFBOs();

    void setSceneUniforms(SupportCanvas3D *context, std::unique_ptr<CS123::GL::CS123Shader> &shader);\
    void setMatrixUniforms(CS123::GL::Shader *shader, SupportCanvas3D *context);
    void setLights( std::unique_ptr<CS123::GL::CS123Shader> &shader);
    void renderGeometry(std::unique_ptr<CS123::GL::CS123Shader> &shader);

    std::unique_ptr<CS123::GL::CS123Shader> m_phongShader;
    std::unique_ptr<CS123::GL::Shader> m_wireframeShader;
    std::unique_ptr<CS123::GL::Shader> m_normalsShader;
    std::unique_ptr<CS123::GL::Shader> m_normalsArrowShader;

    std::unique_ptr<CS123::GL::CS123Shader> m_occlShader;
    std::unique_ptr<CS123::GL::Shader> m_lightShader;
    std::unique_ptr<CS123::GL::Shader> m_crepShader;
    std::unique_ptr<CS123::GL::Shader> m_mergeShader;

    // FBOs
    std::unique_ptr<CS123::GL::FBO> m_phongFBO;
    std::unique_ptr<CS123::GL::FBO> m_occlFBO;
    std::unique_ptr<CS123::GL::FBO> m_lightFBO;
    std::unique_ptr<CS123::GL::FBO> m_crepFBO;

    // Primitives for flywheel pattern
    std::unique_ptr<CubeShape> m_cube;
    std::unique_ptr<ConeShape> m_cone;
    std::unique_ptr<CylinderShape> m_cylinder;
    std::unique_ptr<SphereShape> m_sphere;

    std::unique_ptr<OpenGLShape> m_quad;

    int m_height;
    int m_width;

};

#endif // SCENEVIEWSCENE_H
