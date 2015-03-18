#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "abstractgem.h"

/**
 * @brief TetrahedronGems are gems with a shape like a tetrahedron. The only difference to AbstracGem is the fact, that a TetrahedronGem has a shape defined.
 */

class TetrahedronGem : public AbstractGem
{
    Q_OBJECT
public:
    explicit TetrahedronGem(QObject *parent = 0);
    virtual ~TetrahedronGem();
};

#endif // TETRAHEDRON_H
