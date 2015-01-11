#include "triangle.h"

#include <QVector3D>

Triangle::Triangle(QObject *parent) :
    QObject(parent)
{
}

Triangle::Triangle(const QVector3D &a,
                   const QVector3D &b,
                   const QVector3D &c,
                   const QVector3D &color) :
    m_a(new QVector3D(a))
  , m_b(new QVector3D(b))
  , m_c(new QVector3D(c))
  , m_color(new QVector3D(color))
  , m_normal(nullptr)
{
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
const QVector3D &Triangle::color() const
{
    return *m_color;
}

void Triangle::setColor(const QVector3D &color)
{
    *m_color = color;
}

const QVector3D &Triangle::normal()
{
    if (!m_normal) {
        m_normal = new QVector3D();
        calculateNormal();
    }
    return *m_normal;
}

QVector3D Triangle::normalizedNormal()
{
    if (!m_normal) {
        m_normal = new QVector3D();
        calculateNormal();
    }
    return m_normal->normalized();
}

void Triangle::calculateNormal()
{
    QVector3D normal = QVector3D::crossProduct(*m_b - *m_a, *m_c - *m_a);
    *m_normal = normal;
}
