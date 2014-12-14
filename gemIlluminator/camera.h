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

    QMatrix4x4 const & view();
    QMatrix4x4 const & viewInverted();
    QMatrix4x4 const & viewProjection();
    QMatrix4x4 const & viewProjectionInverted();
    QMatrix4x4 const & projection();
    QMatrix4x4 const & projectionInverted();

    QVector3D position();
    QVector3D viewDirection();
    QVector3D center();
    QVector3D eye();
    QVector3D up();

    QSize viewport();
    float fovy();
    float zNear();
    float zFar();

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
    void recalculateView();
    void recalculateProjection();
    void recalculateViewProjection();

protected:
    QVector3D *m_eye;
    QVector3D *m_center;
    QVector3D *m_up;
    QSize *m_viewport;
    float m_zNear;
    float m_zFar;
    float m_fovy;

    QMatrix4x4 *m_view;
    QMatrix4x4 *m_viewInverted;
    QMatrix4x4 *m_projection;
    QMatrix4x4 *m_projectionInverted;
    QMatrix4x4 *m_viewProjection;
    QMatrix4x4 *m_viewProjectionInverted;

    bool m_isViewInvalid;
    bool m_isProjectionInvalid;
};

#endif // CAMERA_H
