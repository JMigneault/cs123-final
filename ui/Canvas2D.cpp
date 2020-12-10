/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <assert.h>
#include <iostream>
#include <math.h>
#include <memory>
#include <unistd.h>
#include "Canvas2D.h"
#include "Settings.h"
#include "RayScene.h"

#include <QCoreApplication>
#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"
#include "filter/sobelfilter.h"
#include "filter/blurfilter.h"
#include "filter/scalefilter.h"
#include "ray/rayhelpers.h"

#include <glm/gtx/transform.hpp>

Canvas2D::Canvas2D() :
    // @TODO: Initialize any pointers in this class here.
    m_rayScene(nullptr),
    m_brush(new ConstantBrush(settings.brushColor, settings.brushRadius)),
    m_current_radius(settings.brushRadius),
    m_brush_type(BRUSH_CONSTANT)
{
}

Canvas2D::~Canvas2D()
{
}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {
}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.
    SupportCanvas2D::paintEvent(e);

}

void Canvas2D::settingsChanged() {
    // TODO: Process changes to the application settings.
    std::cout << "Canvas2d::settingsChanged() called. Settings have changed" << std::endl;

    m_brush->setRGBA(settings.brushColor);

    if (m_current_radius != settings.brushRadius) {
        m_current_radius = settings.brushRadius;
        m_brush->setRadius(m_current_radius); // Note: calls makeMask()
    }

    // just construct a new brush whenever a different mode is selected
    if (settings.brushType != m_brush_type) {
        m_brush_type = settings.brushType;
        switch (m_brush_type) {
        case BRUSH_CONSTANT:
            m_brush.reset(new ConstantBrush(settings.brushColor, m_current_radius));
            break;
        case BRUSH_LINEAR:
            m_brush.reset(new LinearBrush(settings.brushColor, m_current_radius));
            break;
        case BRUSH_QUADRATIC:
            m_brush.reset(new QuadraticBrush(settings.brushColor, m_current_radius));
            break;
        case BRUSH_SMUDGE:
            m_brush.reset(new SmudgeBrush(settings.brushColor, m_current_radius));
            break;


        }
    }

}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************


void Canvas2D::mouseDown(int x, int y) {
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel

    std::cout << "Canvas2d::mouseDown() called" << std::endl;


//    bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit

    m_brush->brushDown(x, y, this);
    SupportCanvas2D::update();

}

void Canvas2D::mouseDragged(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    std::cout << "Canvas2d::mouseDragged() called" << std::endl;

    m_brush->brushDragged(x, y, this);
    SupportCanvas2D::update();

}

void Canvas2D::mouseUp(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    std::cout << "Canvas2d::mouseUp() called" << std::endl;

    m_brush->brushUp(x, y, this);
    SupportCanvas2D::update();
}



// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage() {
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.

    switch(settings.filterType) {
        case FILTER_BLUR:
            BlurFilter::filter(this, settings.blurRadius);
            break;
        case FILTER_EDGE_DETECT:
            SobelFilter::filter(this, settings.edgeDetectSensitivity);
            break;
        case FILTER_SCALE:
            ScaleFilter::filter(this, settings.scaleX, settings.scaleY);
            break;
            // fill in the rest
    }

    // Leave this code here! This code ensures that the Canvas2D will be completely wiped before
    // drawing the new image.
    repaint(rect());
    QCoreApplication::processEvents();
}

// ********************************************************************************************
// ** RAY
// ********************************************************************************************

void Canvas2D::setScene(RayScene *scene) {
    m_rayScene.reset(scene);
}


void Canvas2D::renderImage(Camera *camera, int width, int height) {
/*
    void RayHelpers::renderImage(Camera *camera, int width, int height, RGBA *data, CS123SceneGlobalData global,
                                 std::vector<CS123SceneLightData> lights, std::vector<CS123ScenePrimitive> primitives,
                                 std::vector<glm::mat4> transformations, std::vector<glm::mat4> inverses) {
*/

    if (m_rayScene) {
        this->resize(width, height);

        std::vector<CS123SceneLightData> lights = m_rayScene->getLights();
        int s = lights.size();


        RayHelpers::renderImage(camera, width, height, this,
                                m_rayScene->getGlobalData(), m_rayScene->getLights(),
                                m_rayScene->getPrimitives(), m_rayScene->getTransformations(),
                                m_rayScene->getInverseTransformations());
    }

        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INTERSECT/RAY

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering.

}

void Canvas2D::cancelRender() {
    // TODO: cancel the raytracer (optional)
}




