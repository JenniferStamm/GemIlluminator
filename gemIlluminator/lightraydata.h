#ifndef LIGHTRAYDATA_H
#define LIGHTRAYDATA_H

#include <QObject>
#include <QVector3D>    //TODO: Remove as soon as I know how to return a calculated QVector3D in best practice

class QVector3D;
class LightRay;
/**
 * @brief The LightRayData class stores data of a LightRay. The LightRayData doesn't inherit
 * from QObject, so it can be stored in Qt-Containers (currently only those require == or qHash) by Value.
 */
class LightRayData
{
public:
    LightRayData();
    LightRayData(const LightRay &lightRay);
    LightRayData(const LightRayData &lightRay);
    ~LightRayData();

    QVector3D normalizedOrthogonalVector() const;

    const QVector3D & startPosition() const;
    void setStartPosition(const QVector3D & position);
    const QVector3D & endPosition() const;
    void setEndPosition(const QVector3D & position);
    const QVector3D & direction() const;
    const QVector3D & normalizedDirection() const;

    LightRayData & operator=(const LightRayData &lightRay);
protected:
    QVector3D *m_startPosition;
    QVector3D *m_endPosition;
    QVector3D *m_direction;
    QVector3D *m_normalizedDirection;

private:
    void setDirection(const QVector3D direction);
};

bool operator==(const LightRayData &ray1, const LightRayData &ray2);
uint qHash(const LightRayData &key, uint seed);

#endif // LIGHTRAYDATA_H