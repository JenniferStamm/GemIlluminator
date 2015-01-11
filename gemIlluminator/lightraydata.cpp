#include "lightraydata.h"

#include <QHash>

#include "lightray.h"

LightRayData::LightRayData() :
    m_startPosition(new QVector3D())
  , m_endPosition(new QVector3D())
  , m_direction(new QVector3D())
  , m_normalizedDirection(new QVector3D())
{
}

LightRayData::LightRayData(const LightRay &ray) :
    m_startPosition(new QVector3D(ray.startPosition()))
  , m_endPosition(new QVector3D(ray.endPosition()))
  , m_direction(new QVector3D(ray.direction()))
  , m_normalizedDirection(new QVector3D(ray.normalizedDirection()))
{
}

LightRayData::LightRayData(const LightRayData &ray) :
    m_startPosition(new QVector3D(ray.startPosition()))
  , m_endPosition(new QVector3D(ray.endPosition()))
  , m_direction(new QVector3D(ray.direction()))
  , m_normalizedDirection(new QVector3D(ray.normalizedDirection()))
{
}

LightRayData::~LightRayData()
{
    delete m_startPosition;
    delete m_endPosition;
    delete m_direction;
    delete m_normalizedDirection;
}

QVector3D LightRayData::normalizedOrthogonalVector() const
{
    if (direction().x() != 0.f) {
        if (direction().z() != 0.f) {
            return QVector3D(1.f, 0.f, - direction().x() / direction().z()).normalized();
        } else {
            return QVector3D(0.f, 1.f, 0.f);
        }
    } else if (direction().y() != 0.f) {
        if (direction().z() != 0.f) {
            return QVector3D(0.f, 1.f, - direction().y() / direction().z()).normalized();
        } else {
            return QVector3D(1.f, 0.f, 0.f);
        }
    } else if (direction().z() != 0.f) {
        return QVector3D(1.f, 0.f, 0.f);
    } else {
        return QVector3D(0.f, 0.f, 0.f);
    }
}

const QVector3D & LightRayData::startPosition() const
{
    return *m_startPosition;
}

void LightRayData::setStartPosition(const QVector3D &position)
{
    if (*m_startPosition == position) {
        return;
    }
    *m_startPosition = position;
    setDirection(*m_endPosition - *m_startPosition);
}

const QVector3D & LightRayData::endPosition() const
{
    return *m_endPosition;
}

void LightRayData::setEndPosition(const QVector3D &position)
{
    if (*m_endPosition == position) {
        return;
    }
    *m_endPosition = position;
    setDirection(*m_endPosition - *m_startPosition);
}

const QVector3D &LightRayData::direction() const
{
    return *m_direction;
}

const QVector3D &LightRayData::normalizedDirection() const
{
    return *m_normalizedDirection;
}

LightRayData & LightRayData::operator=(const LightRayData &lightRay)
{
    if (this != &lightRay)
    {
        delete m_startPosition;
        delete m_endPosition;
        m_startPosition = new QVector3D(lightRay.startPosition());
        m_endPosition = new QVector3D(lightRay.endPosition());
    }
    return *this;
}

void LightRayData::setDirection(const QVector3D direction)
{
    *m_direction = direction;
    *m_normalizedDirection = direction.normalized();
}

bool operator==(const LightRayData &ray1, const LightRayData &ray2)
{
    if(ray1.startPosition() == ray2.startPosition()
            && ray1.endPosition() == ray2.endPosition()) {
        return true;
    }
    return false;
}

uint qHash(const LightRayData &key, uint seed)
{
    return qHash(key.startPosition().x() + key.startPosition().y() + key.startPosition().z() +
                 key.endPosition().x() + key.endPosition().y() + key.endPosition().z(), seed);
}