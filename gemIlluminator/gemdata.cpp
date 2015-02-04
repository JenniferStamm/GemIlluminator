#include "gemdata.h"

#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>

#include "abstractgem.h"
#include "triangle.h"

GemData::GemData() :
    m_color(new QVector3D())
  , m_isModelInvalid(true)
  , m_model(new QMatrix4x4())
  , m_position(new QVector3D())
  , m_rotation(new QQuaternion())
  , m_scale(1.f)
  , m_triangles(new QList<Triangle *>())
  , m_type(Gem::Type::Abstract)
{
}

GemData::GemData(const GemData &otherGemData) :
    m_color(new QVector3D(otherGemData.color()))
  , m_isModelInvalid(true)
  , m_model(new QMatrix4x4())
  , m_position(new QVector3D(otherGemData.position()))
  , m_rotation(new QQuaternion(otherGemData.rotation()))
  , m_scale(otherGemData.scale())
  , m_triangles(new QList<Triangle *>())
  , m_type(otherGemData.type())
{
    copyTriangles(otherGemData.triangles());
}

GemData::GemData(const AbstractGem &gem) :
    m_color(new QVector3D(gem.color()))
  , m_isModelInvalid(true)
  , m_model(new QMatrix4x4())
  , m_position(new QVector3D(gem.position()))
  , m_rotation(new QQuaternion(gem.rotation()))
  , m_scale(gem.scale())
  , m_triangles(new QList<Triangle *>())
  , m_type(gem.type())
{
    copyTriangles(gem.triangles());
}

GemData::~GemData()
{
    delete m_color;
    delete m_model;
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
    m_isModelInvalid = true;
    *m_position = rhs.position();
    *m_rotation = rhs.rotation();
    m_scale = rhs.scale();
    copyTriangles(rhs.triangles());
    m_type = rhs.type();
    return *this;
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

const QMatrix4x4 &GemData::model() const
{
    if (m_isModelInvalid) {
        calculateModelMatrix();
    }
    return *m_model;
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
    m_isModelInvalid = true;
}

const QQuaternion &GemData::rotation() const
{
    return *m_rotation;
}

void GemData::setRotation(const QQuaternion &rotation)
{
    if (*m_rotation == rotation) {
        return;
    }
    *m_rotation = rotation;
    m_isModelInvalid = true;
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
    m_isModelInvalid = true;
}

const QList<Triangle *> &GemData::triangles() const
{
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
    for (auto triangle : *m_triangles) {
        delete triangle;
    }
    m_triangles->clear();
    for (auto triangle : triangles) {
        m_triangles->append(new Triangle(*triangle));
    }
}

void GemData::calculateModelMatrix() const
{
    m_model->setToIdentity();
    m_model->translate(position());
    m_model->scale(scale());
    m_model->rotate(rotation());
    m_isModelInvalid = false;
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
