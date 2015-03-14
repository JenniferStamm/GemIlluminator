#ifndef GEM_H
#define GEM_H

#include "abstractgem.h"

/**
 * @brief CubeGems are gems with a shape like a cube. The only difference to AbstracGem is the fact, that a cube gem has a shape defined.
 */

class CubeGem : public AbstractGem
{
    Q_OBJECT

public:
    explicit CubeGem(QObject *parent = 0);
    virtual ~CubeGem();
};

#endif // GEM_H
