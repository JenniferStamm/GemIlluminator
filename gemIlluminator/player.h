#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "camera.h"

class QVector3D;

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(qreal velocity READ velocity WRITE setVelocity NOTIFY velocityChanged)

public:
    explicit Player(QObject *parent = 0);
    virtual ~Player();

    void setPosition(const QVector3D &position);
    const QVector3D & position();

    void setViewDirection(const QVector3D &viewDirection);

signals:
    void cameraChanged();
    void velocityChanged();

public slots:
    Camera* camera();
    void setCamera(Camera *camera);

    qreal velocity();
    void setVelocity(qreal velocity);

protected:
    qreal m_velocity;
    QVector3D *m_position;
    Camera *m_camera;
};

#endif // PLAYER_H
