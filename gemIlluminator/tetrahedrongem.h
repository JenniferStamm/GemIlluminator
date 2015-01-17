#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "abstractgem.h"

template <typename T> class QVector;
class QVector3D;

class Triangle;

class TetrahedronGem : public AbstractGem
{
    Q_OBJECT
public:
    explicit TetrahedronGem(QObject *parent = 0);
    virtual ~TetrahedronGem();

    virtual void synchronize() override;
    virtual void cleanup() override;
    virtual void update(int timeDifference) override;

    virtual float rayIntersect(const LightRay &ray, int &triangleIndex, QVector3D *collisionPoint = nullptr);

protected:
    QVector<Triangle*> *m_triangles;
    QVector<QVector3D> *m_colors;
    QVector<QVector3D> *m_vertices;
};

#endif // TETRAHEDRON_H
