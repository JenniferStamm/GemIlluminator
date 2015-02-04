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

    virtual void synchronize() override;
    virtual void cleanup() override;

    /**
     * @brief Method does nothing, because bounds are invisible. But method has to be implemented, because no renderer exists.
     * @param gl Unused
     * @param viewProjection Unused
     * @param program Unused
     */
    virtual void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program) override;

    virtual void setInitialRotation(const QQuaternion &initialRotation) override;
    virtual void setPosition(const QVector3D &position) override;
    virtual void setRotation(const QQuaternion &rotation) override;
};

#endif // SCENEBOUNDS_H
