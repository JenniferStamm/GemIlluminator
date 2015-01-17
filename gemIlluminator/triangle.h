#ifndef TRIANGLE_H
#define TRIANGLE_H

class QVector3D;

class AbstractGem;

class Triangle
{
public:
    Triangle(AbstractGem *owningGem);

    Triangle(const QVector3D &a,
             const QVector3D &b,
             const QVector3D &c,
             const QVector3D &color,
             AbstractGem *owningGem);

    Triangle(const Triangle &triangle);

    ~Triangle();

    Triangle &operator=(const Triangle &triangle);

    const QVector3D &a() const;
    void setA(const QVector3D &a);

    const QVector3D &b() const;
    void setB(const QVector3D &b);

    const QVector3D &c() const;
    void setC(const QVector3D &c);

    const QVector3D &color() const;
    void setColor(const QVector3D &color);

    const QVector3D &normal() const;

    QVector3D normalizedNormal() const;

    Triangle inWorldCoordinates() const;

    AbstractGem *owningGem() const;

protected:
    void calculateNormal() const;

protected:
    QVector3D *m_a;
    QVector3D *m_b;
    QVector3D *m_c;
    QVector3D *m_color;
    mutable QVector3D *m_normal;
    AbstractGem *m_gem;
};

#endif // TRIANGLE_H
