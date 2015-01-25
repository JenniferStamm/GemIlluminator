#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QObject>
#include <QQuaternion>

class Navigation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    explicit Navigation(QObject *parent = 0);

    QQuaternion rotation();
    void setRotation(QQuaternion rotation);

    Q_INVOKABLE void setRotationFromEuler(const QVector3D &eulerAngles);

signals:
    void rotationChanged();

protected:
    QQuaternion m_rotation;
};

#endif // NAVIGATION_H
