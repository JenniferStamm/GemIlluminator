#include "camera.h"

#include <QMatrix4x4>

Camera::Camera(QObject *parent) :
    QObject(parent)
  , m_eye(new QVector3D())
  , m_center(new QVector3D())
  , m_up(new QVector3D())
  , m_viewport(new QSize())
  , m_view(new QMatrix4x4())
  , m_viewInverted(new QMatrix4x4())
  , m_projection(new QMatrix4x4())
  , m_projectionInverted(new QMatrix4x4())
  , m_viewProjection(new QMatrix4x4())
  , m_viewProjectionInverted(new QMatrix4x4())
{
}

Camera::~Camera()
{
    delete m_view;
    delete m_viewInverted;
    delete m_projection;
    delete m_projectionInverted;
    delete m_viewProjection;
    delete m_viewProjectionInverted;
    delete m_eye;
    delete m_center;
    delete m_up;
    delete m_viewport;
}

void Camera::recalculateView()
{
    m_view->setToIdentity();
    m_view->lookAt(eye(), center(), up());
    *m_viewInverted = m_view->inverted();
    m_isViewInvalid = false;
}

void Camera::recalculateProjection()
{
    m_projection->setToIdentity();
    m_projection->perspective(fovy(), viewport().width() / viewport().height(), zNear(), zFar());
    *m_projectionInverted = m_projection->inverted();
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
    *m_viewProjection = projection() * view();
    *m_viewProjectionInverted = m_viewProjection->inverted();
}

QMatrix4x4 const & Camera::view()
{
    if (m_isViewInvalid){
        recalculateView();
    }
    return *m_view;
}

QMatrix4x4 const & Camera::viewInverted()
{
    if (m_isViewInvalid){
        recalculateView();
    }
    return *m_viewInverted;
}

QMatrix4x4 const & Camera::viewProjection()
{
    if (m_isViewInvalid || m_isProjectionInvalid){
        recalculateViewProjection();
    }
    return *m_viewProjection;
}

QMatrix4x4 const & Camera::viewProjectionInverted()
{
    if (m_isViewInvalid || m_isProjectionInvalid){
        recalculateViewProjection();
    }
    return *m_viewProjectionInverted;
}

QMatrix4x4 const & Camera::projection()
{
    if (m_isProjectionInvalid){
        recalculateProjection();
    }
    return *m_projection;
}

QMatrix4x4 const & Camera::projectionInverted()
{
    if (m_isProjectionInvalid){
        recalculateProjection();
    }
    return *m_projectionInverted;
}

QVector3D Camera::position()
{
    return *m_eye;
}

QVector3D Camera::viewDirection()
{
    return *m_center - *m_eye;
}

QVector3D Camera::center()
{
    return *m_center;
}

QVector3D Camera::eye()
{
    return *m_eye;
}

QVector3D Camera::up()
{
    return *m_up;
}

QSize Camera::viewport()
{
    return *m_viewport;
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
    if (eye() == position) {
        return;
    }

    QVector3D viewDirection = center() - eye();
    *m_eye = position;
    *m_center = eye() + viewDirection;
    m_isViewInvalid = true;
}

void Camera::setViewDirection(QVector3D viewDirection)
{
    if (center() == eye() + viewDirection){
        return;
    }
    *m_center = eye() + viewDirection;
    m_isViewInvalid = true;
}

void Camera::setEye(QVector3D eye)
{
    if (this->eye() == eye) {
        return;
    }
    *m_eye = eye;
    m_isViewInvalid = true;
}

void Camera::setCenter(QVector3D center)
{
    if (this->center() == center) {
        return;
    }
    *m_center = center;
    m_isViewInvalid = true;
}

void Camera::setUp(QVector3D up)
{
    if (this->up() == up) {
        return;
    }
    *m_up = up;
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
    if (this->viewport() == viewport) {
        return;
    }
    *m_viewport = viewport;
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
