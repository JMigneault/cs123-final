#ifndef VIEW_H
#define VIEW_H

#include "GL/glew.h"
#include <qgl.h>
#include <QTime>
#include <QTimer>

#include <glm.hpp>
#include "gl/datatype/fbo.h"

class OpenGLShape;

class View : public QGLWidget {
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:
    QTime m_time;
    QTimer m_timer;
    bool m_captureMouse;

    // added
    std::unique_ptr<OpenGLShape> m_testShapes[5];
    std::unique_ptr<OpenGLShape> m_quad;
    GLuint m_testProgram;
    GLuint m_occlProgram;
    GLuint m_lightProgram;
    GLuint m_crepProgram;
    glm::mat4 m_model[5];
    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::vec3 m_colors[5];
    glm::vec3 m_skyColor;
    float m_weight;
    float m_secondsPassed;
    float m_displacement;
    boolean m_isPlaying;
    std::unique_ptr<GL::FBO> m_testFBO;
    std::unique_ptr<GL::FBO> m_occlFBO;
    std::unique_ptr<GL::FBO> m_lightFBO;
    std::unique_ptr<GL::FBO> m_crepFBO;
    float m_width;
    float m_height;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    // added
    void drawTest();
    void setParticleViewport();

private slots:
    void tick();
};

#endif // VIEW_H
