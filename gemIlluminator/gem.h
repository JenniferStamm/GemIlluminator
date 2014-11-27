#ifndef GEM_H
#define GEM_H

#include "abstractgeometry.h"

class Gem : public AbstractGeometry
{
public:
    explicit Gem(QObject *parent = 0);
    virtual ~Gem();

    virtual void synchronize() override;
    virtual void cleanup() override;
};

#endif // GEM_H
