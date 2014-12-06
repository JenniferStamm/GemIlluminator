#include "gemrenderer.h"

#include <QMatrix4x4>
#include <QVector>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

GemRenderer::GemRenderer(QObject *parent):
    AbstractGeometryRenderer(parent)
,   m_vertices(new QOpenGLBuffer())
,   m_program(nullptr)
{
}

GemRenderer::~GemRenderer()
{
    m_vertices->destroy();
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
    QVector<float> normal1 = calculateNormal(vector1, vector3, vector4);
    vertexData += vector1;
    vertexData += color1;
    vertexData += normal1;
    vertexData += vector3;
    vertexData += color1;
    vertexData += normal1;
    vertexData += vector4;
    vertexData += color1;
    vertexData += normal1;

    // second triangle
    QVector<float> normal2 = calculateNormal(vector3, vector4, vector2);
    vertexData += vector3;
    vertexData += color2;
    vertexData += normal2;
    vertexData += vector4;
    vertexData += color2;
    vertexData += normal2;
    vertexData += vector2;
    vertexData += color2;
    vertexData += normal2;

    // third triangle
    QVector<float> normal3 = calculateNormal(vector4, vector2, vector1);
    vertexData += vector4;
    vertexData += color3;
    vertexData += normal3;
    vertexData += vector2;
    vertexData += color3;
    vertexData += normal3;
    vertexData += vector1;
    vertexData += color3;
    vertexData += normal3;

    // fourth triangle
    QVector<float> normal4 = calculateNormal(vector2, vector1, vector3);
    vertexData += vector2;
    vertexData += color4;
    vertexData += normal4;
    vertexData += vector1;
    vertexData += color4;
    vertexData += normal4;
    vertexData += vector3;
    vertexData += color4;
    vertexData += normal4;

    return vertexData;
}

QVector<float> GemRenderer::calculateNormal(
        QVector<float> vector1,
        QVector<float> vector2,
        QVector<float> vector3)
{
    QVector3D vector3D1(vector1[0], vector1[1], vector1[2]);
    QVector3D vector3D2(vector2[0], vector2[1], vector2[2]);
    QVector3D vector3D3(vector3[0], vector3[1], vector3[2]);

    QVector3D normal3D = QVector3D::crossProduct(vector3D2 - vector3D1, vector3D3 - vector3D1);
    normal3D.normalize();
    QVector<float> normal{normal3D.x(), normal3D.y(), normal3D.z()};
    return normal;
}

void GemRenderer::paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection)
{
    if (!m_program) {
        initialize();
    }

    m_vertices->bind();
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

    m_vertices->release();
    m_program->release();
}
