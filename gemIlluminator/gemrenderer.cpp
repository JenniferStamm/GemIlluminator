#include "gemrenderer.h"

#include <QDebug>

#include <QMatrix4x4>
#include <QVector>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

GemRenderer::GemRenderer(QVector<QVector3D> *vertices, QVector<QVector3D> *colors, QObject *parent):
    AbstractGeometryRenderer(parent)
,   m_vertexData(new QVector<float>())
,   m_vertexBuffer(new QOpenGLBuffer())
{
    m_vertexData = initializeData(
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
    m_vertexBuffer->allocate(m_vertexData->constData(), m_vertexData->size() * sizeof(float));

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vgem.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fgem.glsl");
    if (!m_program->link()) {
        //close();
    }
}

QVector<float>* GemRenderer::initializeData(
        QVector3D vector1,
        QVector3D vector2,
        QVector3D vector3,
        QVector3D vector4,
        QVector3D color1,
        QVector3D color2,
        QVector3D color3,
        QVector3D color4)
{
    /* Order according to
     * http://math.stackexchange.com/questions/183030/given-a-tetrahedron-how-to-find-the-outward-surface-normals-for-each-side
     */
    QVector<QVector3D> *data = new QVector<QVector3D>();

    // first triangle
    addTriangleData(vector2, vector4, vector3, color1, data);

    // second triangle
    addTriangleData(vector1, vector4, vector3, color2, data);

    // third triangle
    addTriangleData(vector1, vector2, vector4, color3, data);
    QVector3D normal3 = calculateNormal(vector1, vector2, vector4);

    // fourth triangle
    addTriangleData(vector1, vector3, vector2, color4, data);

    #ifdef QT_DEBUG
        qDebug() << "<<<<<<<<<<<<<>>>>>>>>>>>>>";
        qDebug() << "INITIALIZE VERTEX DATA";
        auto i = 0;
        for (auto& vector : *data) {
            if ( i%9 == 0)
                qDebug() << "New triangle: ";
            if ( i%3 == 0)
                qDebug() << "Vertex: " << vector;
            else if ( i%3 == 1)
                qDebug() << "Color: " << vector;
            else if ( i%3 == 2)
                qDebug() << "Normal: " << vector;
            i++;
        }
        qDebug() << "<<<<<<<<<<<<<>>>>>>>>>>>>>";
    #endif

    QVector<float> *dataFloat = new QVector<float>();

    for (auto& i : *data) {
        dataFloat->append(i.x());
        dataFloat->append(i.y());
        dataFloat->append(i.z());
    }

    return dataFloat;
}

void GemRenderer::addTriangleData(
            QVector3D vector1,
            QVector3D vector2,
            QVector3D vector3,
            QVector3D color,
            QVector<QVector3D>* data) {
    if (data == nullptr) {
        #ifdef QT_DEBUG
            qDebug() << "<<<<<<<<<<<<<>>>>>>>>>>>>>";
            qDebug() << "ADD TRIANGLE DATA";
            qDebug() << "Something went very wrong, if data is a nullptr";
            qDebug() << "<<<<<<<<<<<<<>>>>>>>>>>>>>";
        #endif
        return;
    }

    QVector3D normal = calculateNormal(vector1, vector2, vector3);
    *data += vector1;
    *data += color;
    *data += normal;
    *data += vector2;
    *data += color;
    *data += normal;
    *data += vector3;
    *data += color;
    *data += normal;
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

    QMatrix4x4 normalMatrix(model);
    normalMatrix.setColumn(3, QVector4D(0.0, 0.0, 0.0, 1.0));
    normalMatrix.setRow(3, QVector4D(0.0, 0.0, 0.0, 1.0));
    m_program->setUniformValue("normalMatrix", normalMatrix.inverted().transposed());

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


    gl->glDrawArrays(GL_TRIANGLES, 0, 12);

    gl->glDisableVertexAttribArray(0);
    gl->glDisableVertexAttribArray(1);
    gl->glDisableVertexAttribArray(2);

    m_vertexBuffer->release();
    m_program->release();
}
