#include "gemrenderer.h"

#include <QMatrix4x4>
#include <QVector>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

GemRenderer::GemRenderer(QVector<QVector3D> *vertices, QVector<QVector3D> *colors, QObject *parent):
    AbstractGeometryRenderer(parent)
,   m_vertexData(new QVector<QVector3D>())
,   m_vertexBuffer(new QOpenGLBuffer())
{
    m_vertexData = initializeVertexData(
                vertices->at(0),
                vertices->at(1),
                vertices->at(2),
                vertices->at(3),
                colors->at(0),
                colors->at(1),
                colors->at(2),
                colors->at(3)
            );
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
    m_vertexBuffer->allocate(m_vertexData->constData(), m_vertexData->size() * sizeof(float) * 3);

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vgem.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fgem.glsl");
    if (!m_program->link()) {
        //close();
    }
}

QVector<QVector3D>* GemRenderer::initializeVertexData(
        QVector3D vector1,
        QVector3D vector2,
        QVector3D vector3,
        QVector3D vector4,
        QVector3D color1,
        QVector3D color2,
        QVector3D color3,
        QVector3D color4)
{
    QVector<QVector3D> *vertexData = new QVector<QVector3D>();

    // first triangle
    QVector3D normal1 = calculateNormal(vector1, vector3, vector4);
    *vertexData += vector1;
    *vertexData += color1;
    *vertexData += normal1;
    *vertexData += vector3;
    *vertexData += color1;
    *vertexData += normal1;
    *vertexData += vector4;
    *vertexData += color1;
    *vertexData += normal1;

    // second triangle
    QVector3D normal2 = calculateNormal(vector3, vector4, vector2);
    *vertexData += vector3;
    *vertexData += color2;
    *vertexData += normal2;
    *vertexData += vector4;
    *vertexData += color2;
    *vertexData += normal2;
    *vertexData += vector2;
    *vertexData += color2;
    *vertexData += normal2;

    // third triangle
    QVector3D normal3 = calculateNormal(vector4, vector2, vector1);
    *vertexData += vector4;
    *vertexData += color3;
    *vertexData += normal3;
    *vertexData += vector2;
    *vertexData += color3;
    *vertexData += normal3;
    *vertexData += vector1;
    *vertexData += color3;
    *vertexData += normal3;

    // fourth triangle
    QVector3D normal4 = calculateNormal(vector2, vector1, vector3);
    *vertexData += vector2;
    *vertexData += color4;
    *vertexData += normal4;
    *vertexData += vector1;
    *vertexData += color4;
    *vertexData += normal4;
    *vertexData += vector3;
    *vertexData += color4;
    *vertexData += normal4;

    return vertexData;
}

QVector3D GemRenderer::calculateNormal(
        QVector3D vector1,
        QVector3D vector2,
        QVector3D vector3)
{
    QVector3D normal = QVector3D::crossProduct(vector2 - vector1, vector3 - vector1);
    normal.normalize();
    return normal;
}

void GemRenderer::paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection)
{
    if (!m_program) {
        initialize();
    }

    m_vertexBuffer->bind();
    m_program->bind();


    QMatrix4x4 model;
    model.scale(0.5);
    model.translate(m_position.x(), m_position.y(), m_position.z());
    model.rotate(m_rotation.x() + m_initialRotation.x(), QVector3D(1.0, 0.0, 0.0));
    model.rotate(m_rotation.y() + m_initialRotation.y(), QVector3D(0.0, 1.0, 0.0));
    model.rotate(m_rotation.z() + m_initialRotation.z(), QVector3D(0.0, 0.0, 1.0));
    m_program->setUniformValue("model", model);
    m_program->setUniformValue("modelIT", model.inverted().transposed());

    m_program->setUniformValue("viewProjection", viewProjection);

    QMatrix4x4 mvp = viewProjection * model;
    m_program->setUniformValue("modelViewProjection", mvp);

    gl->glEnableVertexAttribArray(0);
    gl->glEnableVertexAttribArray(1);
    gl->glEnableVertexAttribArray(2);

    m_program->bindAttributeLocation("a_vertex", 0);
    m_program->bindAttributeLocation("a_color", 1);
    m_program->bindAttributeLocation("a_normal", 2);

    gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
    gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (3 * sizeof(float)));
    gl->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *) (6 * sizeof(float)));


    gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);

    gl->glDisableVertexAttribArray(0);
    gl->glDisableVertexAttribArray(1);
    gl->glDisableVertexAttribArray(2);

    m_vertexBuffer->release();
    m_program->release();
}
