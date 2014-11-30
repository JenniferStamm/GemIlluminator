#include "camera.h"

Camera::Camera(QObject *parent) :
    QObject(parent)
{
}

void Camera::recalculateView()
{
    m_view.setToIdentity();
    m_view.lookAt(m_eye, m_center, m_up);
    m_viewInverted = m_view.inverted();
    m_isViewInvalid = false;
}

void Camera::recalculateProjection()
{
    m_projection.setToIdentity();
    m_projection.perspective(m_fovy, m_viewport.width() / m_viewport.height(), m_zNear, m_zFar);
    m_projectionInverted = m_projection.inverted();
    m_isViewInvalid = false;
}

void Camera::recalculateViewProjection()
{
    if (m_isViewInvalid){
        recalculateView();
    }
    if (m_isProjectionInvalid){
        recalculateProjection();
    }
    m_viewProjection = m_projection * m_view;
    m_viewProjectionInverted = m_viewProjection.inverted();
}

QMatrix4x4 Camera::view()
{
    if (m_isViewInvalid){
        recalculateView();
    }
    return m_view;
}

QMatrix4x4 Camera::viewInverted()
{
    if (m_isViewInvalid){
        recalculateView();
    }
    return m_viewInverted;
}

QMatrix4x4 Camera::viewProjection()
{
    if (m_isViewInvalid || m_isProjectionInvalid){
        recalculateViewProjection();
    }
    return m_viewProjection;
}

QMatrix4x4 Camera::viewProjectionInverted()
{
    if (m_isViewInvalid || m_isProjectionInvalid){
        recalculateViewProjection();
    }
    return m_viewProjectionInverted;
}

QMatrix4x4 Camera::projection()
{
    if (m_isProjectionInvalid){
        recalculateProjection();
    }
    return m_projection;
}

QMatrix4x4 Camera::projectionInverted()
{
    if (m_isProjectionInvalid){
        recalculateProjection();
    }
    return m_projectionInverted;
}

QVector3D Camera::position()
{
    return m_eye;
}

QVector3D Camera::viewDirection()
{
    return m_center - m_eye;
}

QVector3D Camera::center()
{
    return m_center;
}

QVector3D Camera::eye()
{
    return m_eye;
}

QVector3D Camera::up()
{
    return m_up;
}

QSize Camera::viewport()
{
    return m_viewport;
}

float Camera::fovy()
{
    return m_fovy;
}

float Camera::zNear()
{
    return m_zNear;
}

float Camera::zFar()
{
    return m_zFar;
}

void Camera::setPosition(QVector3D position)
{
    if (m_eye == position) {
        return;
    }

    QVector3D viewDirection = center() - eye();
    m_eye = position;
    m_center = m_eye + viewDirection;
    m_isViewInvalid = true;
}

void Camera::setViewDirection(QVector3D viewDirection)
{
    if (m_center == eye() + viewDirection){
        return;
    }
    m_center = eye() + viewDirection;
    m_isViewInvalid = true;
}

void Camera::setEye(QVector3D eye)
{
    if (m_eye == eye) {
        return;
    }
    m_eye = eye;
    m_isViewInvalid = true;
}

void Camera::setCenter(QVector3D center)
{
    if (m_center == center) {
        return;
    }
    m_center = center;
    m_isViewInvalid = true;
}

void Camera::setUp(QVector3D up)
{
    if (m_up == up) {
        return;
    }
    m_up = up;
    m_isViewInvalid = true;
}

void Camera::setView(QVector3D eye, QVector3D center, QVector3D up)
{
    setEye(eye);
    setCenter(center);
    setUp(up);
}

void Camera::setViewport(QSize viewport)
{
    if (m_viewport == viewport) {
        return;
    }
    m_viewport = viewport;
    m_isProjectionInvalid = true;
}

void Camera::setViewport(int x, int y)
{
    setViewport(QSize(x, y));
}

void Camera::setFovy(float angle)
{
    if (m_fovy == angle) {
        return;
    }
    m_fovy = angle;
    m_isProjectionInvalid = true;
}

void Camera::setZNear(float zNear)
{
    if (m_zNear == zNear) {
        return;
    }
    m_zNear = zNear;
    m_isProjectionInvalid = true;
}

void Camera::setZFar(float zFar)
{
    if (m_zFar == zFar){
        return;
    }
    m_zFar = zFar;
    m_isProjectionInvalid = true;
}
