#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;

class Camera;
class ScreenAlignedQuad;

class CubeMap : public QObject
{
    Q_OBJECT
public:
    CubeMap(QOpenGLFunctions &gl, QString cubeMapPrefix,QObject *parent = 0);
    virtual ~CubeMap();

    void update(QString newCubeMapPrefix);

    void paint(const Camera &camera);

    uint cubeMapTexture();

protected:
    void initialize();
    void initializeShaderProgram();

protected:
    uint m_cubeMap;
    QOpenGLFunctions &m_gl;
    QString m_cubeMapPrefix;
    bool m_initialized;
    ScreenAlignedQuad *m_quad;
    QOpenGLShaderProgram *m_shaderProgram;
};

#endif // ENVIRONMENTMAP_H
