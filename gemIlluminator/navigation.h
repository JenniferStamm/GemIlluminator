#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QObject>
#include <QQuaternion>

class Navigation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuaternion rotateX READ rotateX WRITE setRotateX NOTIFY rotateXChanged)
    Q_PROPERTY(QQuaternion rotateY READ rotateY WRITE setRotateY NOTIFY rotateYChanged)
    Q_PROPERTY(QQuaternion rotateZ READ rotateZ WRITE setRotateZ NOTIFY rotateZChanged)

public:
    explicit Navigation(QObject *parent = 0);

    QQuaternion rotateX();
    void setRotateX(QQuaternion rotateX);

    QQuaternion rotateY();
    void setRotateY(QQuaternion rotateY);

    QQuaternion rotateZ();
    void setRotateZ(QQuaternion rotateZ);

signals:
    void rotateXChanged();
    void rotateYChanged();
    void rotateZChanged();

protected:
    QQuaternion m_rotateX;
    QQuaternion m_rotateY;
    QQuaternion m_rotateZ;
};

#endif // NAVIGATION_H
