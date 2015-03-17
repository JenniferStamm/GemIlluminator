#pragma once

#include <QOpenGLBuffer>

class QOpenGLFunctions;

/**
 * @brief The ScreenAlignedQuad class encapsulates the drawing of a screen aligned quad.
 */
class ScreenAlignedQuad
{
public:
    /**
     * @brief Creates a new ScreenAlignedQuad
     */
    ScreenAlignedQuad();
    ~ScreenAlignedQuad();

    /**
     * @brief Draws the ScreenAlignedQuad with bound shader program.
     * @param gl QOpenGLFunctions that are used to do gl-calls
     */
    void draw(QOpenGLFunctions &gl);

protected:
    /**
     * @brief Writes vertices into given buffer.
     * @param vertices The buffer into which the vertices are written
     */
    static void strip(QOpenGLBuffer &vertices);

private:
    QOpenGLBuffer *m_vertices;
};
