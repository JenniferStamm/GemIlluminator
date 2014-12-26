#ifndef LIGHTRAYDATA_H
#define LIGHTRAYDATA_H

#include <QObject>

class QVector3D;
class LightRay;
/**
 * @brief The LightRayData class stores data of a LightRay. The LightRayData doesn't inherit
 * from QObject, so it can be stored in Qt-Containers (currently only those require == or qHash) by Value.
 */
class LightRayData
{
public:
    LightRayData(const LightRay &lightRay);
    LightRayData(const LightRayData &lightRay);
    ~LightRayData();

    const QVector3D & startPosition() const;
    const QVector3D & endPosition() const;

    LightRayData & operator=(const LightRayData &lightRay);
protected:
    QVector3D *m_startPosition;
    QVector3D *m_endPosition;
};

bool operator==(const LightRayData &ray1, const LightRayData &ray2);
uint qHash(const LightRayData &key, uint seed);

#endif // LIGHTRAYDATA_H
