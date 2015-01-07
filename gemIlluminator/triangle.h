#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QObject>

class QVector3D;

class Triangle : public QObject
{
    Q_OBJECT
public:
    explicit Triangle(QObject *parent = 0);
    Triangle(const QVector3D *a,
             const QVector3D *b,
             const QVector3D *c,
             const QVector3D *color);



    const QVector3D *a() const;
    void setA(const QVector3D *a);

    const QVector3D *b() const;
    void setB(const QVector3D *b);

    const QVector3D *c() const;
    void setC(const QVector3D *c);

    const QVector3D *color() const;
    void setColor(const QVector3D *color);

protected:
    const QVector3D *m_a;
    const QVector3D *m_b;
    const QVector3D *m_c;
    const QVector3D *m_color;
};

#endif // TRIANGLE_H
