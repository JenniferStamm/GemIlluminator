#include "triangle.h"

#include <cassert>

#include <QMatrix4x4>
#include <QVector3D>

#include "abstractgem.h"

Triangle::Triangle(AbstractGem *owningGem) :
    m_a(new QVector3D())
  , m_b(new QVector3D())
  , m_c(new QVector3D())
  , m_color(new QVector3D())
  , m_normal(nullptr)
  , m_gem(owningGem)
{
    assert(m_gem);
}

Triangle::Triangle(const QVector3D &a,
                   const QVector3D &b,
                   const QVector3D &c,
                   const QVector3D &color,
                   AbstractGem *gem) :
    m_a(new QVector3D(a))
  , m_b(new QVector3D(b))
  , m_c(new QVector3D(c))
  , m_color(new QVector3D(color))
  , m_normal(nullptr)
  , m_gem(gem)
{
    assert(gem);
}

Triangle::Triangle(const Triangle &triangle):
    m_a(new QVector3D(triangle.a()))
  , m_b(new QVector3D(triangle.b()))
  , m_c(new QVector3D(triangle.c()))
  , m_color(new QVector3D(triangle.color()))
  , m_normal(nullptr)
  , m_gem(triangle.owningGem())
{
    assert(m_gem);
}

Triangle &Triangle::operator=(const Triangle &triangle)
{
    delete m_a;
    delete m_b;
    delete m_c;
    delete m_color;
    delete m_normal;

    m_a = new QVector3D(triangle.a());
    m_b = new QVector3D(triangle.b());
    m_c = new QVector3D(triangle.c());
    m_color = new QVector3D(triangle.color());
    m_normal = nullptr;
    m_gem = triangle.owningGem();

    return *this;
}

Triangle::~Triangle()
{
    delete m_a;
    delete m_b;
    delete m_c;
    delete m_color;
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
const QVector3D &Triangle::color() const
{
    return *m_color;
}

void Triangle::setColor(const QVector3D &color)
{
    *m_color = color;
}

const QVector3D &Triangle::normal() const
{
    if (!m_normal) {
        m_normal = new QVector3D();
        calculateNormal();
    }
    return *m_normal;
}

QVector3D Triangle::normalizedNormal() const
{
    if (!m_normal) {
        m_normal = new QVector3D();
        calculateNormal();
    }
    return m_normal->normalized();
}

Triangle Triangle::inWorldCoordinates() const
{
    assert(m_gem);
    QMatrix4x4 model;
    model.translate(m_gem->position().x(), m_gem->position().y(), m_gem->position().z());
    model.scale(m_gem->scale());
    model.rotate(m_gem->rotation().x() + m_gem->initialRotation().x(), QVector3D(1.0, 0.0, 0.0));
    model.rotate(m_gem->rotation().y() + m_gem->initialRotation().y(), QVector3D(0.0, 1.0, 0.0));
    model.rotate(m_gem->rotation().z() + m_gem->initialRotation().z(), QVector3D(0.0, 0.0, 1.0));

    Triangle result(m_gem);
    result.setA(model * a());
    result.setB(model * b());
    result.setC(model * c());
    result.setColor(color());
    return result;
}

AbstractGem *Triangle::owningGem() const
{
    return m_gem;
}

QVector3D Triangle::reflect(const QVector3D &incidentVector) const
{
    QVector3D N = inWorldCoordinates().normalizedNormal();
    QVector3D I = incidentVector.normalized();
    return I - 2.0 * QVector3D::dotProduct(N, I) * N;
}

void Triangle::calculateNormal() const
{
    QVector3D normal = QVector3D::crossProduct(*m_b - *m_a, *m_c - *m_a);
    *m_normal = normal;
}
