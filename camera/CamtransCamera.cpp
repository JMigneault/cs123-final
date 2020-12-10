/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>

CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...
    setClip(1, 30);
    m_thetaH = glm::radians(60.0f);
    setAspectRatio(1.0f);
    glm::vec4 eye = glm::vec4(2, 2, 2, 1);
    orientLook(eye, glm::normalize(glm::vec4(0, 0, 0, 1) - eye), glm::vec4(0, 1, 0, 0));

}

void CamtransCamera::setAspectRatio(float a)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_aspectRatio = a;

    float height = 2 * m_far * glm::tan(m_thetaH / 2);
    float width = m_aspectRatio * height;
    m_thetaW = 2 * glm::atan(width / (2 * m_far));

    updateProjectionMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return getPerspectiveMatrix() * getScaleMatrix();
    // return glm::mat4x4();
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_rotationMatrix * m_translationMatrix;

}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_eye;

}

glm::vec4 CamtransCamera::getLook() const {
    // @TODO: [CAMTRANS] Fill this in...
    return -m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_up;
}

float CamtransCamera::getAspectRatio() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_thetaH;
}

glm::vec4 CamtransCamera::getU() const {
    return m_u;
}

glm::vec4 CamtransCamera::getV() const {
    return m_v;
}

glm::vec4 CamtransCamera::getW() const {
    return m_w;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    // @TODO: [CAMTRANS] Fill this in...
    m_eye = eye;
    m_up = up;

    m_w = glm::normalize(-look);
    m_v = glm::normalize(up - glm::dot(up, m_w) * m_w);
    m_u = glm::normalize(glm::vec4(glm::cross(m_v.xyz(), m_w.xyz()), 0));

    m_up = m_v;

    updateViewMatrix();
    updateProjectionMatrix();

}

// expects h in degrees
void CamtransCamera::setHeightAngle(float h) {
    // @TODO: [CAMTRANS] Fill this in...
    m_thetaH = glm::radians(h);
    float height = 2 * m_far * glm::tan(m_thetaH / 2);
    float width = m_aspectRatio * height;
    m_thetaW = 2 * glm::atan(width / (2 * m_far));

    updateProjectionMatrix();

}

void CamtransCamera::translate(const glm::vec4 &v) {
    // @TODO: [CAMTRANS] Fill this in...
    m_eye += v;

    updateViewMatrix();

}

void CamtransCamera::rotateU(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    float theta = glm::radians(degrees);
    glm::vec4 newV = m_w * glm::sin(theta) + m_v * glm::cos(theta);
    glm::vec4 newW = m_w * glm::cos(theta) - m_v * glm::sin(theta);
    m_v = newV;
    m_w = newW;

    m_up = m_v;

    updateViewMatrix();

}

void CamtransCamera::rotateV(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    float theta = glm::radians(degrees);
    glm::vec4 newU = m_u * glm::cos(theta) - m_w * glm::sin(theta);
    glm::vec4 newW = m_u * glm::sin(theta) + m_w * glm::cos(theta);
    m_u = newU;
    m_w = newW;

    updateViewMatrix();

}

void CamtransCamera::rotateW(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    float theta = glm::radians(degrees);
    glm::vec4 newU = m_u * glm::cos(theta) + m_v * glm::sin(theta);
    glm::vec4 newV = -m_u * glm::sin(theta) + m_v * glm::cos(theta);
    m_u = newU;
    m_v = newV;

    m_up = m_v;

    updateViewMatrix();

}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    // @TODO: [CAMTRANS] Fill this in...
    m_near = nearPlane;
    m_far = farPlane;

    updateProjectionMatrix();

}

void CamtransCamera::updateProjectionMatrix() {
    updateScaleMatrix();
    updatePerspectiveMatrix();
}
void CamtransCamera::updatePerspectiveMatrix() {
    float c = - m_near / m_far;
    m_perspectiveTransformation = glm::mat4(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, -1 / (c + 1), -1,
                0, 0, c / (c + 1), 0);
}
void CamtransCamera::updateScaleMatrix() {
    m_scaleMatrix = glm::mat4(
                1 / (m_far * glm::tan(m_thetaW / 2)), 0, 0, 0,
                0, 1 / (m_far * glm::tan(m_thetaH / 2)), 0, 0,
                0, 0, 1 / m_far, 0,
                0, 0, 0, 1);
}
void CamtransCamera::updateViewMatrix() {
    updateRotationMatrix();
    updateTranslationMatrix();
}

void CamtransCamera::updateRotationMatrix() {
    m_rotationMatrix = glm::mat4(m_u.x, m_v.x, m_w.x, 0,
                                 m_u.y, m_v.y, m_w.y, 0,
                                 m_u.z, m_v.z, m_w.z, 0,
                                 0, 0, 0, 1);
}

void CamtransCamera::updateTranslationMatrix() {
    glm::vec4 pos = getPosition();
    m_translationMatrix = glm::mat4(1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    -pos.x, -pos.y, -pos.z, 1);
}

