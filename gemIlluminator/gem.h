#ifndef GEM_H
#define GEM_H

#include "abstractgem.h"

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

protected:
    QVector<QVector3D> *m_vertices;
    QVector<QVector3D> *m_colors;
};

#endif // GEM_H
