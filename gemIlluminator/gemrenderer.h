#ifndef GEMRENDERER_H
#define GEMRENDERER_H

#include "abstractgeometryrenderer.h"

class GemRenderer : public AbstractGeometryRenderer
{
public:
    explicit GemRenderer(QObject *parent = 0);

    void paint() override;
};

#endif // GEMRENDERER_H
