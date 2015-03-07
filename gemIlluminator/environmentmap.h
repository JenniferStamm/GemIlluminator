#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;

class Camera;
class CubeMap;
class ScreenAlignedQuad;

class EnvironmentMap : public QObject
{
    Q_OBJECT
public:
    explicit EnvironmentMap(QOpenGLFunctions &gl, QString cubeMapPrefix, QObject *parent = 0);
    ~EnvironmentMap();

    void paint(const Camera &camera);

    void update(QString newCubeMapPrefix);

    uint cubeMapTexture();

protected:
    void initialize();
    void initializeShaderProgram();

protected:
    CubeMap *m_cubeMap;
    bool m_initialized;
    QOpenGLFunctions &m_gl;
    ScreenAlignedQuad *m_quad;
    QOpenGLShaderProgram *m_shaderProgram;
};

#endif // ENVIRONMENTMAP_H
