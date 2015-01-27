#include "camera.h"

#include <QMatrix4x4>

Camera::Camera(QObject *parent) :
    QObject(parent)
  , m_eye(new QVector3D())
  , m_center(new QVector3D())
  , m_up(new QVector3D())
  , m_viewport(new QSize())
  , m_zNear(0.1f)
  , m_zFar(10.0f)
  , m_fovy(60.f)
  , m_view(new QMatrix4x4())
  , m_viewInverted(new QMatrix4x4())
  , m_projection(new QMatrix4x4())
  , m_projectionInverted(new QMatrix4x4())
  , m_viewProjection(new QMatrix4x4())
  , m_viewProjectionInverted(new QMatrix4x4())
  , m_isViewInvalid(true)
  , m_isProjectionInvalid(true)
  , m_isViewProjectionInvalid(true)
{
}

Camera::Camera(const Camera &camera, QObject *parent) :
    QObject(parent)
  , m_eye(new QVector3D(camera.eye()))
  , m_center(new QVector3D(camera.center()))
  , m_up(new QVector3D(camera.up()))
  , m_viewport(new QSize(camera.viewport()))
  , m_zNear(camera.zNear())
  , m_zFar(camera.zFar())
  , m_fovy(camera.fovy())
  , m_view(new QMatrix4x4(camera.view()))
  , m_viewInverted(new QMatrix4x4(camera.viewInverted()))
  , m_projection(new QMatrix4x4(camera.projection()))
  , m_projectionInverted(new QMatrix4x4(camera.projectionInverted()))
  , m_viewProjection(new QMatrix4x4(camera.viewProjection()))
  , m_viewProjectionInverted(new QMatrix4x4(camera.viewProjectionInverted()))
  , m_isViewInvalid(false)
  , m_isProjectionInvalid(false)
  , m_isViewProjectionInvalid(false)
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

void Camera::recalculateView() const
{
    m_view->setToIdentity();
    m_view->lookAt(eye(), center(), up());
    *m_viewInverted = m_view->inverted();
    m_isViewInvalid = false;
}

void Camera::recalculateProjection() const
{
    m_projection->setToIdentity();
    m_projection->perspective(fovy(), static_cast<float>(viewport().width()) / viewport().height(), zNear(), zFar());
    *m_projectionInverted = m_projection->inverted();
    m_isProjectionInvalid = false;
}

void Camera::recalculateViewProjection() const
{
    if (m_isViewInvalid){
        recalculateView();
    }
    if (m_isProjectionInvalid){
        recalculateProjection();
    }
    *m_viewProjection = projection() * view();
    *m_viewProjectionInverted = m_viewProjection->inverted();
    m_isViewProjectionInvalid = false;
}

QMatrix4x4 const & Camera::view() const
{
    if (m_isViewInvalid){
        recalculateView();
    }
    return *m_view;
}

QMatrix4x4 const & Camera::viewInverted() const
{
    if (m_isViewInvalid){
        recalculateView();
    }
    return *m_viewInverted;
}

QMatrix4x4 const & Camera::viewProjection() const
{
    if (m_isViewProjectionInvalid){
        recalculateViewProjection();
    }
    return *m_viewProjection;
}

QMatrix4x4 const & Camera::viewProjectionInverted() const
{
    if (m_isViewProjectionInvalid){
        recalculateViewProjection();
    }
    return *m_viewProjectionInverted;
}

QMatrix4x4 const & Camera::projection() const
{
    if (m_isProjectionInvalid){
        recalculateProjection();
    }
    return *m_projection;
}

QMatrix4x4 const & Camera::projectionInverted() const
{
    if (m_isProjectionInvalid){
        recalculateProjection();
    }
    return *m_projectionInverted;
}

const QVector3D &Camera::position() const
{
    return *m_eye;
}

QVector3D Camera::viewDirection() const
{
    return *m_center - *m_eye;
}

const QVector3D &Camera::center() const
{
    return *m_center;
}

const QVector3D &Camera::eye() const
{
    return *m_eye;
}

const QVector3D &Camera::up() const
{
    return *m_up;
}

const QSize &Camera::viewport() const
{
    return *m_viewport;
}

float Camera::fovy() const
{
    return m_fovy;
}

float Camera::zNear() const
{
    return m_zNear;
}

float Camera::zFar() const
{
    return m_zFar;
}

void Camera::setPosition(const QVector3D &position)
{
    if (eye() == position) {
        return;
    }

    QVector3D viewDirection = center() - eye();
    *m_eye = position;
    *m_center = eye() + viewDirection;
    invalidateView();
}

void Camera::setViewDirection(const QVector3D &viewDirection)
{
    if (center() == eye() + viewDirection){
        return;
    }
    *m_center = eye() + viewDirection;
    invalidateView();
}

void Camera::setEye(const QVector3D &eye)
{
    if (this->eye() == eye) {
        return;
    }
    *m_eye = eye;
    invalidateView();
}

void Camera::setCenter(const QVector3D &center)
{
    if (this->center() == center) {
        return;
    }
    *m_center = center;
    invalidateView();
}

void Camera::setUp(const QVector3D &up)
{
    if (this->up() == up) {
        return;
    }
    *m_up = up;
    invalidateView();
}

void Camera::setView(const QVector3D &eye, const QVector3D &center, const QVector3D &up)
{
    setEye(eye);
    setCenter(center);
    setUp(up);
}

void Camera::setViewport(const QSize &viewport)
{
    if (*m_viewport == viewport) {
        return;
    }
    *m_viewport = viewport;
    invalidateProjection();
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
    invalidateProjection();
}

void Camera::setZNear(float zNear)
{
    if (m_zNear == zNear) {
        return;
    }
    m_zNear = zNear;
    invalidateProjection();
}

void Camera::setZFar(float zFar)
{
    if (m_zFar == zFar){
        return;
    }
    m_zFar = zFar;
    invalidateProjection();
}

void Camera::invalidateView() const
{
    m_isViewInvalid = true;
    m_isViewProjectionInvalid = true;
}

void Camera::invalidateProjection() const
{
    m_isProjectionInvalid = true;
    m_isViewProjectionInvalid = true;
}
