#ifndef LIGHTRAY_H
#define LIGHTRAY_H

#include <QObject>
#include <QVector3D>
#include "player.h"

template <typename T> class QList;
class QVector3D;
class LightRayRenderer;

class LightRay : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QVector3D & startPosition READ startPosition WRITE setStartPosition NOTIFY startPositionChanged)
    Q_PROPERTY(const QVector3D & endPosition READ endPosition WRITE setEndPosition NOTIFY endPositionChanged)
    Q_PROPERTY(const QVector3D & direction READ direction)
    Q_PROPERTY(Player * player READ player WRITE setPlayer NOTIFY playerChanged)

public:
    explicit LightRay(QObject *parent = 0);

    /**
     * @brief Pseudo-Copy-Constructor
     * @param lightRay Lightray to be copied
     * @param parent QObject-Parent
     */
    LightRay(LightRay & lightRay, QObject *parent = 0);
    virtual ~LightRay();

    virtual void synchronize();
    virtual void cleanup();
    virtual void update(int timeDifference);

signals:
    void startPositionChanged();
    void endPositionChanged();
    void playerChanged();

public slots:
    const QVector3D & startPosition();
    void setStartPosition(const QVector3D & position);

    const QVector3D & endPosition();
    void setEndPosition(const QVector3D & position);

    const QVector3D & direction();

    Player * player();
    void setPlayer(Player *attachedPlayer);

protected:
    virtual void _synchronize(LightRayRenderer *renderer);

protected:
    QList<LightRay *> *m_successors;
    LightRayRenderer *m_renderer;

    Player *m_player;
    QVector3D *m_startPosition;
    QVector3D *m_endPosition;
    QVector3D *m_direction;
};

#endif // LIGHTRAY_H
