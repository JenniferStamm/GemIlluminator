#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QObject>

class QVector3D;

class Triangle : public QObject
{
    Q_OBJECT
public:
    explicit Triangle(QObject *parent = 0);
    Triangle(const QVector3D &a,
             const QVector3D &b,
             const QVector3D &c,
             const QVector3D &color);

    const QVector3D &a() const;
    void setA(const QVector3D &a);

    const QVector3D &b() const;
    void setB(const QVector3D &b);

    const QVector3D &c() const;
    void setC(const QVector3D &c);

    const QVector3D &color() const;
    void setColor(const QVector3D &color);

    const QVector3D &normal();

    QVector3D normalizedNormal();

protected:
    void calculateNormal();

protected:
    QVector3D *m_a;
    QVector3D *m_b;
    QVector3D *m_c;
    QVector3D *m_color;
    QVector3D *m_normal;
};

#endif // TRIANGLE_H
