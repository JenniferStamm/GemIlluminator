#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QSize>
#include <QVector3D>

class QMatrix4x4;

class Camera : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D eye READ eye WRITE setEye)
    Q_PROPERTY(QVector3D center READ center WRITE setCenter)
    Q_PROPERTY(QVector3D up READ up WRITE setUp)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition)
    Q_PROPERTY(QVector3D viewDirection READ viewDirection WRITE setViewDirection)
    Q_PROPERTY(QSize viewport READ viewport WRITE setViewport)
    Q_PROPERTY(qreal zNear READ zNear WRITE setZNear)
    Q_PROPERTY(qreal zFar READ zFar WRITE setZFar)
    Q_PROPERTY(qreal fovy READ fovy WRITE setFovy)

public:
    explicit Camera(QObject *parent = 0);

    /** \brief Creates a new Camera with matrices copied.
     *  \param camera Specifies the camera the matrices are copied from.
     *  \param parent Specifies the parent
     */
    Camera(Camera & camera, QObject *parent = 0);
    virtual ~Camera();

    QMatrix4x4 const & view() const;
    QMatrix4x4 const & viewInverted() const;
    QMatrix4x4 const & viewProjection() const;
    QMatrix4x4 const & viewProjectionInverted() const;
    QMatrix4x4 const & projection() const;
    QMatrix4x4 const & projectionInverted() const;

    QVector3D position() const;
    QVector3D viewDirection() const;
    QVector3D center() const;
    QVector3D eye() const;
    QVector3D up() const;

    QSize viewport() const;
    float fovy() const;
    float zNear() const;
    float zFar() const;

signals:
    void viewChanged();

public slots:
    void setPosition(QVector3D position);
    void setViewDirection(QVector3D viewDirection);
    void setEye(QVector3D eye);
    void setCenter(QVector3D center);
    void setUp(QVector3D up);
    void setView(QVector3D eye, QVector3D center, QVector3D up);

    void setViewport(QSize viewport);
    void setViewport(int x, int y);
    void setFovy(float angle);
    void setZNear(float zNear);
    void setZFar(float zFar);

protected:
    void recalculateView() const;
    void recalculateProjection() const;
    void recalculateViewProjection() const;

protected:
    QVector3D *m_eye;
    QVector3D *m_center;
    QVector3D *m_up;
    QSize *m_viewport;
    float m_zNear;
    float m_zFar;
    float m_fovy;

    mutable QMatrix4x4 *m_view;
    mutable QMatrix4x4 *m_viewInverted;
    mutable QMatrix4x4 *m_projection;
    mutable QMatrix4x4 *m_projectionInverted;
    mutable QMatrix4x4 *m_viewProjection;
    mutable QMatrix4x4 *m_viewProjectionInverted;

    mutable bool m_isViewInvalid;
    bool m_isProjectionInvalid;
};

#endif // CAMERA_H
