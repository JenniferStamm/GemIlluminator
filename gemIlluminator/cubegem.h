#ifndef GEM_H
#define GEM_H

#include "abstractgem.h"

class CubeGem : public AbstractGem
{
    Q_OBJECT

public:
    explicit CubeGem(QObject *parent = 0);
    virtual ~CubeGem();

    virtual void synchronize() override;
    virtual void cleanup() override;
};

#endif // GEM_H
