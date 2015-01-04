#ifndef LIGHTRAY_H
#define LIGHTRAY_H

#include <QObject>
#include <QVector3D>
#include "player.h"

template <typename T> class QList;
class QOpenGLFunctions;
class QVector3D;

class LightRayRenderer;
class Scene;

class LightRay : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QVector3D & startPosition READ startPosition WRITE setStartPosition NOTIFY startPositionChanged)
    Q_PROPERTY(const QVector3D & endPosition READ endPosition WRITE setEndPosition NOTIFY endPositionChanged)
    Q_PROPERTY(const QVector3D & direction READ direction)
    Q_PROPERTY(const QVector3D & normalizedDirection READ normalizedDirection)
    Q_PROPERTY(Player * player READ player WRITE setPlayer NOTIFY playerChanged)
    Q_PROPERTY(Scene * scene READ scene WRITE setScene NOTIFY sceneChanged)

public:
    explicit LightRay(QObject *parent = 0);
    virtual ~LightRay();

    virtual void synchronize();
    virtual void cleanup();
    virtual void update(int timeDifference);

signals:
    void startPositionChanged();
    void endPositionChanged();
    void playerChanged();
    void sceneChanged();

public slots:
    const QVector3D & startPosition() const;
    void setStartPosition(const QVector3D & position);

    const QVector3D & endPosition() const;
    void setEndPosition(const QVector3D & position);

    const QVector3D & direction() const;

    const QVector3D & normalizedDirection() const;

    Player * player();
    void setPlayer(Player *attachedPlayer);

    Scene * scene();
    void setScene(Scene *owningScene);

    bool isStatic() const;
    void setStatic();

    void paint(QOpenGLFunctions *gl);

protected:
    virtual void _synchronize(LightRayRenderer *renderer);

protected:
    QList<LightRay *> *m_successors;
    LightRayRenderer *m_renderer;
    bool m_isStatic;

    Player *m_player;
    Scene *m_scene;
    QVector3D *m_startPosition;
    QVector3D *m_endPosition;
    QVector3D *m_direction;
    QVector3D *m_normalizedDirection;

private:
    void setDirection(const QVector3D direction);
};

#endif // LIGHTRAY_H
