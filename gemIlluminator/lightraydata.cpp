#include "lightraydata.h"

#include <QHash>

#include "lightray.h"

LightRayData::LightRayData(const LightRay &ray) :
    m_startPosition(new QVector3D(ray.startPosition()))
  , m_endPosition(new QVector3D(ray.endPosition()))
{
}

LightRayData::LightRayData(const LightRayData &ray) :
    m_startPosition(new QVector3D(ray.startPosition()))
  , m_endPosition(new QVector3D(ray.endPosition()))
{
}

LightRayData::~LightRayData()
{
    delete m_startPosition;
    delete m_endPosition;
}

const QVector3D & LightRayData::startPosition() const
{
    return *m_startPosition;
}

const QVector3D & LightRayData::endPosition() const
{
    return *m_endPosition;
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
