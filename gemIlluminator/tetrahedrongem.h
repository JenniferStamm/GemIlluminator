#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "abstractgem.h"

class TetrahedronGem : public AbstractGem
{
    Q_OBJECT
public:
    explicit TetrahedronGem(QObject *parent = 0);
    virtual ~TetrahedronGem();

    virtual void synchronize() override;
    virtual void cleanup() override;
    virtual void update(int timeDifference) override;
};

#endif // TETRAHEDRON_H
