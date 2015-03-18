#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <QObject>

class QOpenGLFunctions;

/**
 * @brief The CubeMap class loads cubemap textures and provides them as OpenGL-texture.
 */
class CubeMap : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Creates a new CubeMap and loads required textures.
     * @param gl QOpenGLFunctions that are used for all gl-calls
     * @param cubeMapPrefix The name of the cubemap. This is the filename of textures without fileextension and face suffix.
     * @param parent QObject-parent
     */
    CubeMap(QOpenGLFunctions &gl, QString cubeMapPrefix, QObject *parent = 0);
    virtual ~CubeMap();

    /**
     * @brief Replaces current texture with new specified texture.
     * @param newCubeMapPrefix The name of the new cubemap. This is the filename of textures without fileextension and face suffix.
     */
    void update(QString newCubeMapPrefix);

    /**
     * @brief OpenGL texture reference
     * @return
     */
    uint cubeMapTexture();

protected:
    void initialize();

protected:
    uint m_cubeMap;
    QOpenGLFunctions &m_gl;
    QString m_cubeMapPrefix;
};

#endif // CUBEMAP_H
