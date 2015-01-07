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

protected:
    Triangle *m_triangles[4];
    QVector<QVector3D> *m_vertices;
    QVector<QVector3D> *m_colors;
};

#endif // GEM_H
