#ifndef VIEW_H
#define VIEW_H

#include "GL/glew.h"
#include <qgl.h>
#include <QTime>
#include <QTimer>

#include <glm.hpp>

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
    std::unique_ptr<OpenGLShape> m_testShape;
    GLuint m_testProgram;
    glm::mat4 m_mvp;
    glm::vec3 m_color;

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

private slots:
    void tick();
};

#endif // VIEW_H
