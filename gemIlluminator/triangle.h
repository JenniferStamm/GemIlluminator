#ifndef TRIANGLE_H
#define TRIANGLE_H

template<typename T> class QList;
class QVector3D;

class AbstractGem;

class Triangle
{
public:
    Triangle();

    Triangle(const QVector3D &a,
             const QVector3D &b,
             const QVector3D &c);

    Triangle(const Triangle &triangle);

    ~Triangle();

    Triangle &operator=(const Triangle &triangle);

    const QVector3D &a() const;
    void setA(const QVector3D &a);

    const QVector3D &b() const;
    void setB(const QVector3D &b);

    const QVector3D &c() const;
    void setC(const QVector3D &c);

    const QVector3D &normal() const;

    QList<QVector3D> vertices() const;

    QVector3D reflect(const QVector3D &incidentVector) const;

protected:
    void calculateNormal() const;

protected:
    QVector3D *m_a;
    QVector3D *m_b;
    QVector3D *m_c;
    mutable QVector3D *m_normal;
};

#endif // TRIANGLE_H
