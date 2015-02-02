#include "gemdata.h"

#include <QVector3D>
#include <QQuaternion>

#include "abstractgem.h"

GemData::GemData() :
    m_color(new QVector3D())
  , m_position(new QVector3D())
  , m_rotation(new QQuaternion())
  , m_scale(1.f)
  , m_triangles(new QList<Triangle *>())
{
}

GemData::GemData(const GemData &otherGemData) :
    m_color(new QVector3D(otherGemData.color()))
  , m_position(new QVector3D(otherGemData.position()))
  , m_rotation(new QQuaternion(otherGemData.rotation()))
  , m_scale(otherGemData.scale())
  , m_triangles(new QList<Triangle *>())
{
    copyTriangles(otherGemData.triangles());
}

GemData::GemData(const AbstractGem &gem) :
    m_color(new QVector3D(gem.color()))
  , m_position(new QVector3D(gem.position()))
  , m_rotation(new QQuaternion(gem.rotation()))
  , m_scale(gem.scale())
  , m_triangles(new QList<Triangle *>())
{
    //copyTriangles(gem.);
    //TODO: Copy Triangles
}

GemData::~GemData()
{
    delete m_color;
    delete m_position;
    delete m_rotation;
    for (auto triangle : *m_triangles) {
        delete triangle;
    }
    delete m_triangles;
}

GemData &GemData::operator=(const GemData &rhs)
{
    *m_color = rhs.color();
    *m_position = rhs.position();
    *m_rotation = rhs.rotation();
    copyTriangles(rhs.triangles());
}

const QVector3D &GemData::color() const
{
    return *m_color;
}

void GemData::setColor(const QVector3D &color)
{
    if (*m_color == color) {
        return;
    }
    *m_color = color;
}

const QVector3D &GemData::position() const
{
    return *m_position;
}

void GemData::setPosition(const QVector3D &position)
{
    if (*m_position == position) {
        return;
    }
    *m_position = position;
}

const GemData::QQuaternion &rotation() const
{
    return *m_rotation;
}

void GemData::setRotation(const QQuaternion &rotation)
{
    if (*m_rotation == rotation) {
        return;
    }
    *m_rotation = rotation;
}

float GemData::scale() const
{
    return m_scale;
}

void GemData::setScale(float scale)
{
    if (m_scale == scale) {
        return;
    }
    m_scale = scale;
}

const QList<Triangle *> &GemData::triangles() const
{
    //TODO: Do I really want that to do? Or return a copy would be better?
    return *m_triangles;
}

void GemData::setTriangles(const QList<Triangle *> &triangles)
{
    copyTriangles(triangles);
}

Gem::Type GemData::type() const
{
    return m_type;
}

void GemData::setType(Gem::Type type)
{
    if (m_type == type) {
        return;
    }
    m_type = type;
}

void GemData::copyTriangles(const QList<Triangle *> &triangles)
{
    m_triangles->clear();
    for (auto triangle : triangles) {
        m_triangles->append(new Triangle(*triangle));
    }
}

bool operator==(const GemData &lhs, const GemData &rhs)
{
    if (lhs.type() != rhs.type()) {
        return false;
    }
    if (!qFuzzyCompare(lhs.color(), rhs.color())) {
        return false;
    }
    if (!qFuzzyCompare(lhs.position(), rhs.position())) {
        return false;
    }
    if (!qFuzzyCompare(lhs.rotation(), rhs.rotation())) {
        return false;
    }
    if (!qFuzzyCompare(lhs.scale(), rhs.scale())) {
        return false;
    }
    return true;
}

uint qHash(const GemData &key, uint seed)
{
    return qHash(key.color().x() + key.color().y() + key.color().z() +
                 key.position().x() + key.position().y() + key.position().z() +
                 key.rotation().x() + key.rotation().y() + key.rotation().z() + key.rotation().scalar() +
                 key.scale(), seed);
}
