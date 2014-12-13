#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QObject>

class Navigation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal rotateX READ rotateX WRITE setRotateX NOTIFY rotateXChanged)
    Q_PROPERTY(qreal rotateY READ rotateY WRITE setRotateY NOTIFY rotateYChanged)
    Q_PROPERTY(qreal rotateZ READ rotateZ WRITE setRotateZ NOTIFY rotateZChanged)

public:
    explicit Navigation(QObject *parent = 0);

    qreal rotateX();
    void setRotateX(qreal rotateX);

    qreal rotateY();
    void setRotateY(qreal rotateY);

    qreal rotateZ();
    void setRotateZ(qreal rotateZ);

signals:
    void rotateXChanged();
    void rotateYChanged();
    void rotateZChanged();

protected:
    qreal m_rotateX;
    qreal m_rotateY;
    qreal m_rotateZ;
};

#endif // NAVIGATION_H
