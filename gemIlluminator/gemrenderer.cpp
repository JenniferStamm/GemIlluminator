#include "gemrenderer.h"

#include <QMatrix4x4>
#include <QVector>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

GemRenderer::GemRenderer(QObject *parent):
    AbstractGeometryRenderer(parent)
,   m_vertices(new QOpenGLBuffer())
,   m_program(nullptr)
{

}

void GemRenderer::initialize()
{
    QVector<float> vertex1{-0.5, -0.5, 0.5};
    QVector<float> vertex2{0.5, -0.5, 0.5};
    QVector<float> vertex3{0, -0.5, -0.5};
    QVector<float> vertex4{0.f, 0.5, 0.f};
    QVector<float> color1{1.0, 0.0, 0.0};
    QVector<float> color2{0.0, 1.0, 1.0};
    QVector<float> color3{0.0, 1.0, 0.0};
    QVector<float> color4{0.0, 0.0, 1.0};
    QVector<float> vertexData = initializeVertexData(
                vertex1,
                vertex2,
                vertex3,
                vertex4,
                color1,
                color2,
                color3,
                color4
                );

    m_vertices->create();
    m_vertices->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertices->bind();
    m_vertices->allocate(vertexData.constData(), vertexData.size() * sizeof(float));

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vgem.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fgem.glsl");
    if (!m_program->link()) {
        //close();
    }
}

QVector<float> GemRenderer::initializeVertexData(
        QVector<float> vector1,
        QVector<float> vector2,
        QVector<float> vector3,
        QVector<float> vector4,
        QVector<float> color1,
        QVector<float> color2,
        QVector<float> color3,
        QVector<float> color4)
{
    QVector<float> vertexData;

    // first triangle
    vertexData += vector1;
    vertexData += color1;
    vertexData += vector3;
    vertexData += color1;
    vertexData += vector4;
    vertexData += color1;

    // second triangle
    vertexData += vector3;
    vertexData += color2;
    vertexData += vector4;
    vertexData += color2;
    vertexData += vector2;
    vertexData += color2;

    // third triangle
    vertexData += vector4;
    vertexData += color3;
    vertexData += vector2;
    vertexData += color3;
    vertexData += vector1;
    vertexData += color3;

    // fourth triangle
    vertexData += vector2;
    vertexData += color4;
    vertexData += vector1;
    vertexData += color4;
    vertexData += vector3;
    vertexData += color4;
    return vertexData;
}

void GemRenderer::paint(QOpenGLFunctions *gl)
{
    if (!m_program) {
        initialize();
    }

    m_vertices->bind();
    m_program->bind();

    QMatrix4x4 mvp;

    mvp.translate(0.f, 0.f, 0.5f);
    m_program->setUniformValue("modelViewProjection", mvp);

    gl->glEnableVertexAttribArray(0);
    gl->glEnableVertexAttribArray(1);

    m_program->bindAttributeLocation("a_vertex", 0);
    m_program->bindAttributeLocation("a_color", 1);

    gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

    gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);

    gl->glDisableVertexAttribArray(0);
    gl->glDisableVertexAttribArray(1);

    m_vertices->release();
    m_program->release();
}
