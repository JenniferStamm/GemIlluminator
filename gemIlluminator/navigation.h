#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QObject>
#include <QVector3D>

class QQuaternion;

class Camera;

class Navigation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QVector3D &eulerRotation READ eulerRotation WRITE setEulerRotation NOTIFY eulerRotationChanged)

public:
    explicit Navigation(QObject *parent = 0);
    virtual ~Navigation();

    QQuaternion rotation() const;
    QQuaternion worldSpaceRotation() const;

    void setCamera(Camera *camera);

    const QVector3D &eulerRotation() const;
    void setEulerRotation(const QVector3D &angles);

signals:
    void rotationChanged();
    void eulerRotationChanged();

protected:
    QQuaternion fromEulerAngleQuaternions(const QQuaternion &x, const QQuaternion &y, const QQuaternion &z) const;

protected:
    Camera *m_camera;
    QVector3D *m_eulerRotation;
};

#endif // NAVIGATION_H
