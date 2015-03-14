#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QQmlEngine>

#include "fileio.h"

class Config : public FileIO
{
    Q_OBJECT
    Q_PROPERTY(int axisRange READ axisRange WRITE setAxisRange NOTIFY axisRangeChanged)
    Q_PROPERTY(QString envMap READ envMap WRITE setEnvMap NOTIFY envMapChanged)
    Q_PROPERTY(float maxGemSize READ maxGemSize WRITE setMaxGemSize NOTIFY maxGemSizeChanged)
    Q_PROPERTY(float minGemSize READ minGemSize WRITE setMinGemSize NOTIFY minGemSizeChanged)

    Config(const Config &) = delete;
    Config& operator=(const Config &) = delete;
public:
    virtual ~Config();

    int axisRange();
    static void drop();
    QString envMap() const;
    static Config *instance();
    float maxGemSize() const;
    float minGemSize() const;
    void setAxisRange(int& axisRange);
    void setEnvMap(const QString &envMap);
    void setMaxGemSize(float maxGemSize);
    void setMinGemSize(float minGemSize);

signals:
    void axisRangeChanged();
    void envMapChanged();
    void maxGemSizeChanged();
    void minGemSizeChanged();

protected:
    Config();

protected:
    int m_axisRange;
    QString m_envMap;
    static Config *m_instance;
    float m_maxGemSize;
    float m_minGemSize;
};

QObject *configSingletontypeProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

#endif // CONFIG_H
