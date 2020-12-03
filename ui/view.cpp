#include "view.h"

#include "viewformat.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>
#include "cs123_lib/resourceloader.h"
#include "cs123_lib/sphere.h"
#include <glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "openglshape.h"
#include "gl/shaders/shaderattriblocations.h"

#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include "gl/textures/texture2d.h"


// Added
#include <QTextStream>

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_time(), m_timer(), m_captureMouse(false),
    m_testShape(nullptr), m_testProgram(0) // added
{
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor
    if (m_captureMouse) {
        QApplication::setOverrideCursor(Qt::BlankCursor);
    }

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The update loop is implemented using a timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
}

View::~View()
{
}

void View::initializeGL() {
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    //initialize glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Something is very wrong, glew initialization failed." << std::endl;
    }
    std::cout << "Using GLEW " <<  glewGetString( GLEW_VERSION ) << std::endl;

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    m_time.start();
    m_timer.start(1000 / 60);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    uint errr = glGetError();

    m_testProgram = ResourceLoader::createShaderProgram(
                ":/shaders/test.vert", ":/shaders/test.frag");

    m_occlProgram = ResourceLoader::createShaderProgram(
                ":/shaders/test.vert", ":/shaders/occl.frag");

    m_lightProgram = ResourceLoader::createShaderProgram(
                ":/shaders/quad.vert", ":/shaders/light.frag");

    m_crepProgram = ResourceLoader::createShaderProgram(
                ":/shaders/quad.vert", ":/shaders/crep.frag");


    std::vector<GLfloat> data = {-0.5f, -.5f, 0,
                                 .5f, -.5f, 0,
                                 0, .5f, 0};

    std::vector<GLfloat> sphereData = SPHERE_VERTEX_POSITIONS;

    m_testShape = std::make_unique<OpenGLShape>();

    errr = glGetError();

    // Initialize VBO
    // m_testShape->setVertexData(&sphereData[0], sphereData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, NUM_SPHERE_VERTICES);
    m_testShape->setVertexData(&data[0], data.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, 3);

    errr = glGetError();

    // Add attributes
    m_testShape->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_testShape->setAttribute(ShaderAttrib::NORMAL, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);

    errr = glGetError();

    // Build VAO
    m_testShape->buildVAO();

    errr = glGetError();

    // Set up uniform values
    m_color = glm::vec3(0.53f, 0.81f, 0.98f);

    glm::mat4 projTrans = glm::perspective(.8f, (float) width() / (float) height(), 0.1f, 100.0f);
    glm::mat4 viewTrans = glm::lookAt(
                glm::vec3(0,0,2.5), // Camera is at (0,0,1), in World Space
                glm::vec3(0,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 modelTrans = glm::translate(glm::vec3(-.7, 0, 0));

    m_mvp = projTrans * viewTrans * modelTrans;

    // Make FBOs
    m_testFBO = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE,
                                      width(), height(),
                                      TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                      TextureParameters::FILTER_METHOD::NEAREST, GL_FLOAT);

    m_occlFBO = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE,
                                      width(), height(),
                                      TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                      TextureParameters::FILTER_METHOD::NEAREST, GL_FLOAT);

    m_lightFBO = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE,
                                      width(), height(),
                                      TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                      TextureParameters::FILTER_METHOD::NEAREST, GL_FLOAT);


    m_crepFBO = std::make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE,
                                      width(), height(),
                                      TextureParameters::WRAP_METHOD::CLAMP_TO_EDGE,
                                      TextureParameters::FILTER_METHOD::NEAREST, GL_FLOAT);

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

    errr = glGetError();


    /*
     * NOTE: attempt to write openGL code w/o TA provided API
     * Done as a learning exercise...
     * Below code does not compile but is good reference for what calls are actually being
     * made to openGL without all the noise of the TA API
     *
     *
    GLenum errr;
    while((errr = glGetError()) != GL_NO_ERROR)
    {
      // Process/log the error.
        fprintf(stdout, "%d\n",  errr);
    }
    GLfloat data[9] = {-0.5f, -.5f, 0, .5f, -.5f, 0, 0, .5f, 0};

    // VBO
    GLuint vboHandle = -1;
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), data, GL_STATIC_DRAW);

    while((errr = glGetError()) != GL_NO_ERROR)
    {
      // Process/log the error.
        fprintf(stdout, "%d\n",  errr);
    }

    // VAO
    GLuint vaoHandle = 0;
    glGenVertexArrays(1, &vaoHandle);

    //bind
    // vbo.bind+enable
    //unbind
    glBindVertexArray(vaoHandle);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);


    while((errr = glGetError()) != GL_NO_ERROR)
    {
      // Process/log the error.
        fprintf(stdout, "%d\n",  errr);
    }

    // add attributes
//    m_sphere->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
//    m_sphere->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);

    const GLuint POSITION = 0;

    glEnableVertexAttribArray(POSITION);
    //     glVertexAttribPointer(NAME, NUMELTS, DATATYPER, DATANORMALIZE, STRIDE, OFFSET)
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 3, reinterpret_cast<GLvoid*>(0));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    while((errr = glGetError()) != GL_NO_ERROR)
    {
      // Process/log the error.
        fprintf(stdout, "%d\n",  errr);
    }

    // bind shaders and draw

    glUseProgram(testShader);

    GLint mvpUniformLoc = glGetUniformLocation(testShader, "mvp");

    // COPIED MVP CODE

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) 4.0 / (float) 3.0, 0.1f, 100.0f);

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model;

    glUniformMatrix4fv(mvpUniformLoc, 1, false, &mvp[0][0]);

    GLint colorUniformLoc = glGetUniformLocation(testShader, "color");
    glm::vec4 color = glm::vec4(255, 255, 255, 255);
    glUniform4fv(colorUniformLoc, 1, &color[0]);


    while((errr = glGetError()) != GL_NO_ERROR)
    {
      // Process/log the error.
        fprintf(stdout, "%d\n",  errr); // 0x502
    }


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 1);

    glBindVertexArray(0);

    while((errr = glGetError()) != GL_NO_ERROR)
    {
      // Process/log the error.
        fprintf(stdout, "%d\n",  errr); // 0x502
    }

    */


}


void View::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render scene to testFBO

    m_testFBO->bind();

    glUseProgram(m_testProgram);

    GLint mvcUniformLoc = glGetUniformLocation(m_testProgram, "mvp");
    GLint colorUniformLoc = glGetUniformLocation(m_testProgram, "color");
    glUniformMatrix4fv(mvcUniformLoc, 1, GL_FALSE, glm::value_ptr(m_mvp));
    glUniform3fv(colorUniformLoc, 1, glm::value_ptr(m_color));

    m_testShape->draw();
    glUseProgram(0);

    m_testFBO->unbind();

    // Render occlusion geometry to occlFBO
    m_occlFBO->bind();

    glUseProgram(m_occlProgram);

    mvcUniformLoc = glGetUniformLocation(m_occlProgram, "mvp");
    glUniformMatrix4fv(mvcUniformLoc, 1, GL_FALSE, glm::value_ptr(m_mvp));

    m_testShape->draw();
    glUseProgram(0);

    m_occlFBO->getColorAttachment(0).bind();
    m_occlFBO->unbind();

    // Render sunlight to lightFBO

    m_lightFBO->bind();

    glUseProgram(m_lightProgram);

    colorUniformLoc = glGetUniformLocation(m_testProgram, "color");
    glUniform3fv(colorUniformLoc, 1, glm::value_ptr(m_color));

    m_quad->draw();
    glUseProgram(0);
    m_occlFBO->getColorAttachment(0).unbind();

    m_lightFBO->getColorAttachment(0).bind();
    m_lightFBO->unbind();

    // Draw crepescular rays to screen (temp)

    //m_crepFBO->bind();

    glUseProgram(m_crepProgram);

    // TODO: set uniforms

    m_quad->draw();

    glUseProgram(0);

    m_lightFBO->getColorAttachment(0).unbind();

    /* m_crepFBO->unbind();

    // Final render merging crep rays and phong shader scene

    m_crepFBO->getColorAttachment(0).bind();
    m_testFBO->getColorAttachment(0).bind();

    glUseProgram(m_crepProgram);

    m_quad->draw();

    glUseProgram(0); */






}

void View::resizeGL(int w, int h) {
    float ratio = static_cast<QGuiApplication *>(QCoreApplication::instance())->devicePixelRatio();
    w = static_cast<int>(w / ratio);
    h = static_cast<int>(h / ratio);
    glViewport(0, 0, w, h);

    // TODO: change mvp?

}

void View::mousePressEvent(QMouseEvent *event) {

}

void View::mouseMoveEvent(QMouseEvent *event) {
    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
    if(m_captureMouse) {
        int deltaX = event->x() - width() / 2;
        int deltaY = event->y() - height() / 2;
        if (!deltaX && !deltaY) return;
        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));

        // TODO: Handle mouse movements here
    }
}

void View::mouseReleaseEvent(QMouseEvent *event) {

}

void View::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) QApplication::quit();

    // TODO: Handle keyboard presses here
}

void View::keyReleaseEvent(QKeyEvent *event) {

}

void View::tick() {
    // Get the number of seconds since the last tick (variable update rate)
    float seconds = m_time.restart() * 0.001f;

    // TODO: Implement the demo update here

    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}
