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

    void setPosition(const QVector3D &position) override;
    void setRotation(const QQuaternion &rotation) override;

    QList<LightRay *> processRayIntersection(const LightRay &ray, Scene *scene) override;

signals:
    void gameLost();
};

#endif // SCENEBOUNDS_H
