#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QQmlEngine>

class Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int axisRange READ axisRange WRITE setAxisRange NOTIFY axisRangeChanged)
    Q_PROPERTY(QString envMap READ envMap WRITE setEnvMap NOTIFY envMapChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    int axisRange();
    static void drop();
    QString envMap() const;
    static Config *instance();
    Q_INVOKABLE QString read();
    void setAxisRange(int& axisRange);
    void setEnvMap(const QString &envMap);
    void setSource(const QString& source);
    QString source();
    Q_INVOKABLE bool write(const QString& data);

signals:
    void axisRangeChanged();
    void envMapChanged();
    void error(const QString& msg);
    void sourceChanged(const QString& source);

protected:
    Config() {}
    Config(const Config &); // hide copy constructor
    Config& operator=(const Config &); // hide assign op
                                 // we leave just the declarations, so the compiler will warn us
                                 // if we try to use those two functions by accident

    int m_axisRange;
    QString m_envMap;
    static Config *m_instance;
    QString m_source;
};

static QObject *configSingletontypeProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    Config *instance = Config::instance();
    return instance;
}

#endif // CONFIG_H
