#ifndef GEM_H
#define GEM_H

#include "abstractgem.h"

class Triangle;
template <typename T> class QVector;
class QVector3D;

class Gem : public AbstractGem
{
    Q_OBJECT

public:
    explicit Gem(QObject *parent = 0);
    virtual ~Gem();

    virtual void synchronize() override;
    virtual void cleanup() override;
    virtual void update(int timeDifference) override;

    virtual float rayIntersect(const LightRay &ray, int *triangleIndex, QVector3D *collisionPoint = nullptr);

protected:
    QVector<Triangle*> *m_triangles;
    QVector<QVector3D> *m_vertices;
    QVector<QVector3D> *m_colors;
};

#endif // GEM_H
