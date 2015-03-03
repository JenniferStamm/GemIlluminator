#include "triangle.h"

#include <QMatrix4x4>
#include <QVector3D>

#include "abstractgem.h"

Triangle::Triangle() :
    m_a(new QVector3D())
  , m_b(new QVector3D())
  , m_c(new QVector3D())
  , m_normal(nullptr)
{
}

Triangle::Triangle(const QVector3D &a, const QVector3D &b, const QVector3D &c) :
    m_a(new QVector3D(a))
  , m_b(new QVector3D(b))
  , m_c(new QVector3D(c))
  , m_normal(nullptr)
{
}

Triangle::Triangle(const Triangle &triangle):
    m_a(new QVector3D(triangle.a()))
  , m_b(new QVector3D(triangle.b()))
  , m_c(new QVector3D(triangle.c()))
  , m_normal(nullptr)
{
}

Triangle &Triangle::operator=(const Triangle &triangle)
{
    delete m_a;
    delete m_b;
    delete m_c;
    delete m_normal;

    m_a = new QVector3D(triangle.a());
    m_b = new QVector3D(triangle.b());
    m_c = new QVector3D(triangle.c());
    m_normal = nullptr;

    return *this;
}

Triangle::~Triangle()
{
    delete m_a;
    delete m_b;
    delete m_c;
    delete m_normal;
}

const QVector3D &Triangle::a() const
{
    return *m_a;
}

void Triangle::setA(const QVector3D &a)
{
    *m_a = a;
}

const QVector3D &Triangle::b() const
{
    return *m_b;
}

void Triangle::setB(const QVector3D &b)
{
    *m_b = b;
}

const QVector3D &Triangle::c() const
{
    return *m_c;
}

void Triangle::setC(const QVector3D &c)
{
    *m_c = c;
}

const QVector3D &Triangle::normal() const
{
    if (!m_normal) {
        m_normal = new QVector3D();
        calculateNormal();
    }
    return *m_normal;
}

QList<QVector3D> Triangle::vertices() const
{
    QList<QVector3D> result;
    result.append(a());
    result.append(b());
    result.append(c());
    return result;
}

QVector3D Triangle::reflect(const QVector3D &incidentVector) const
{
    QVector3D N = normal();
    QVector3D I = incidentVector.normalized();
    return I - 2.0 * QVector3D::dotProduct(N, I) * N;
}

void Triangle::calculateNormal() const
{
    QVector3D normal = QVector3D::crossProduct(*m_b - *m_a, *m_c - *m_a);
    *m_normal = normal.normalized();
}
