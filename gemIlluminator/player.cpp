#include "player.h"

#include "lightray.h"

Player::Player(QObject *parent) :
    QObject(parent)
  , m_velocity(1.f)
  , m_position(new QVector3D())
  , m_camera(nullptr)
{
}

Player::~Player()
{
    delete m_position;
}

void Player::moveOnRay(const LightRay &ray, int timeDifferenceInMilliseconds)
{
    *m_position += (ray.normalizedDirection() * velocity() * timeDifferenceInMilliseconds) / 1000;
    updateCameraForPointOnRay(*m_position, ray);
}

void Player::moveToStartPointOnRay(const LightRay &ray)
{
    *m_position = ray.startPosition();
    updateCameraForPointOnRay(*m_position, ray);
}

void Player::moveToEndPointOnRay(const LightRay &ray)
{
    *m_position = ray.endPosition();
    updateCameraForPointOnRay(*m_position, ray);
}

const QVector3D & Player::position()
{
    return *m_position;
}

void Player::setPosition(const QVector3D & position)
{
    if (*m_position == position) {
        return;
    }
    *m_position = position;
    if (m_camera){
        m_camera->setPosition(position + QVector3D(0.f, .1f, 0.f));
    }
}

void Player::setViewDirection(const QVector3D &viewDirection)
{
    if (m_camera){
        m_camera->setViewDirection(viewDirection);
    }
}

Camera* Player::camera()
{
    return m_camera;
}

void Player::setCamera(Camera *camera)
{
    m_camera = camera;
}

qreal Player::velocity()
{
    return m_velocity;
}

void Player::setVelocity(qreal velocity)
{
    if (m_velocity == velocity){
        return;
    }
    m_velocity = velocity;
    emit velocityChanged();
}

void Player::updateCameraForPointOnRay(const QVector3D &point, const LightRay &ray)
{
    QVector3D up = ray.normalizedOrthogonalVector();
    m_camera->setUp(up);
    m_camera->setPosition(point + up * 0.1f);
}
