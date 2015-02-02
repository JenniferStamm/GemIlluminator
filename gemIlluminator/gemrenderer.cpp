#include "gemrenderer.h"

#include "triangle.h"

#include <QDebug>

#include <QMatrix4x4>
#include <QVector>
#include <QVector3D>
#include <QQuaternion>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

GemRenderer::GemRenderer(const QList<Triangle *> &triangles, QObject *parent):
    AbstractGemRenderer(parent)
,   m_initialized(false)
,   m_vertexData(new QVector<float>())
,   m_vertexBuffer(new QOpenGLBuffer())
{
    m_vertexData = initializeData(triangles);
}

GemRenderer::~GemRenderer()
{
    m_vertexBuffer->destroy();
}

void GemRenderer::initialize()
{
    m_vertexBuffer->create();
    m_vertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertexBuffer->bind();
    m_vertexBuffer->allocate(m_vertexData->constData(), m_vertexData->size() * sizeof(float));

    m_initialized = true;
}

QVector<float>* GemRenderer::initializeData(
        const QList<Triangle *> &triangles)
{
    /* Order according to
     * http://math.stackexchange.com/questions/183030/given-a-tetrahedron-how-to-find-the-outward-surface-normals-for-each-side
     */
    QVector<QVector3D> data = QVector<QVector3D>();

    for (auto triangle : triangles) {
        addTriangleData(*triangle, data);
    }

    QVector<float> *dataFloat = new QVector<float>();

    for (auto& i : data) {
        dataFloat->append(i.x());
        dataFloat->append(i.y());
        dataFloat->append(i.z());
    }

    return dataFloat;
}

void GemRenderer::addTriangleData(
            Triangle &triangle,
            QVector<QVector3D> &data)
{
    QVector3D normal = triangle.normalizedNormal();

    data.append(triangle.a());
    data.append(normal);
    data.append(triangle.b());
    data.append(normal);
    data.append(triangle.c());
    data.append(normal);
}

void GemRenderer::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program)
{
    if (!m_initialized) {
        initialize();
    }

    m_vertexBuffer->bind();
    program.bind(); // Ask Daniel why we need this here

    program.setUniformValue("model", model());

    QMatrix4x4 normalMatrix(model());
    normalMatrix.setColumn(3, QVector4D(0.0, 0.0, 0.0, 1.0));
    normalMatrix.setRow(3, QVector4D(0.0, 0.0, 0.0, 1.0));
    program.setUniformValue("normalMatrix", normalMatrix.inverted().transposed());

    program.setUniformValue("viewProjection", viewProjection);

    QMatrix4x4 mvp = viewProjection * model();
    program.setUniformValue("modelViewProjection", mvp);

    program.setUniformValue("color", *m_color);

    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

    gl.glDrawArrays(GL_TRIANGLES, 0, m_vertexData->size() / 6);

    m_vertexBuffer->release();
    program.release();
}
