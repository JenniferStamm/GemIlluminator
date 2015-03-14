#ifndef LIGHTRAYDATA_H
#define LIGHTRAYDATA_H

#include <QObject>

class QVector3D;
class LightRay;
/**
 * @brief The LightRayData class stores data of a LightRay. The LightRayData doesn't inherit
 * from QObject, so it can be stored in Qt-Containers (currently only those require == or qHash) by Value.
 * Also the data can be copied easily.
 */
class LightRayData
{
public:
    LightRayData();
    LightRayData(const LightRay &lightRay);
    LightRayData(const LightRayData &lightRay);
    ~LightRayData();

    /**
     * @brief Calculates a normalized vector, that is orthogonal to direction().
     * @return
     */
    QVector3D normalizedOrthogonalVector() const;

    const QVector3D &color() const;
    void setColor(const QVector3D &color);

    const QVector3D &startPosition() const;
    void setStartPosition(const QVector3D &position);
    const QVector3D &endPosition() const;
    void setEndPosition(const QVector3D &position);
    const QVector3D &direction() const;
    const QVector3D &normalizedDirection() const;

    LightRayData & operator=(const LightRayData &lightRay);
protected:
    QVector3D *m_color;
    QVector3D *m_direction;
    QVector3D *m_directionNormalized;
    QVector3D *m_endPosition;
    QVector3D *m_startPosition;
};

bool operator==(const LightRayData &ray1, const LightRayData &ray2);
uint qHash(const LightRayData &key, uint seed);

#endif // LIGHTRAYDATA_H
