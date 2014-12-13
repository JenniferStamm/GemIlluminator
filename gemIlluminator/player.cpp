#include "player.h"

Player::Player(QObject *parent) :
    QObject(parent)
{
}

Player::~Player()
{
}

void Player::setPosition(const QVector3D & position)
{
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
