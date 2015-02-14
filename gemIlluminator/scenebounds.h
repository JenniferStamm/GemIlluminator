#ifndef SCENEBOUNDS_H
#define SCENEBOUNDS_H

#include "abstractgem.h"

class QMatrix4x4;
class QQuaternion;

class ScreenAlignedQuad;

class SceneBounds : public AbstractGem
{
    Q_OBJECT
public:
    explicit SceneBounds(QObject *parent = 0);
    virtual ~SceneBounds();

    virtual void setPosition(const QVector3D &position) override;
    virtual void setRotation(const QQuaternion &rotation) override;
};

#endif // SCENEBOUNDS_H
