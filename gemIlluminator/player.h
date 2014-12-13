#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "camera.h"

class QVector3D;

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)

public:
    explicit Player(QObject *parent = 0);
    virtual ~Player();

    void setPosition(const QVector3D & position);

signals:
    void cameraChanged();

public slots:
    Camera* camera();
    void setCamera(Camera *camera);

protected:
    QVector3D *m_position;
    //TODO: m_camera should be reference to a Camera. So the m_camera should be set in constructor, but is it possible with QML?
    Camera *m_camera;
};

#endif // PLAYER_H
