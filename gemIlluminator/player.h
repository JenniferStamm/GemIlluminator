#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

#include "camera.h"

class QVector3D;

class LightRay;

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(qreal velocity READ velocity WRITE setVelocity NOTIFY velocityChanged)

public:
    explicit Player(QObject *parent = 0);
    virtual ~Player();

    void moveOnRay(const LightRay &ray, int timeDifferenceInMilliseconds);
    void moveToStartPointOnRay(const LightRay &ray);
    void moveToEndPointOnRay(const LightRay &ray);

    const QVector3D &position() const;
    void setPosition(const QVector3D &position);

    void setViewDirection(const QVector3D &viewDirection);

signals:
    void cameraChanged();
    void velocityChanged();

public slots:
    Camera* camera() const;
    void setCamera(Camera *camera);

    qreal velocity() const;
    void setVelocity(qreal velocity);

protected:
    void updateCameraForPointOnRay(const QVector3D &point, const LightRay & ray);

protected:
    qreal m_velocity;
    QVector3D *m_position;
    Camera *m_camera;
};

#endif // PLAYER_H
