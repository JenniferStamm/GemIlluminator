#include "player.h"

Player::Player(QObject *parent) :
    QObject(parent)
  , m_position(new QVector3D())
  , m_velocity(1.f)
{
}

Player::~Player()
{
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
        m_camera->setPosition(position);
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
    if (m_velocity = velocity){
        return;
    }
    m_velocity = velocity;
    emit velocityChanged();
}
