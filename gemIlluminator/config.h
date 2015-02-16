#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QQmlEngine>

class Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
public:
    static void drop();
    static Config* instance();
    Q_INVOKABLE QString read();
    QString source();
    Q_INVOKABLE bool write(const QString& data);

public slots:
    void setSource(const QString& source);

signals:
    void error(const QString& msg);
    void sourceChanged(const QString& source);

protected:
    QString m_source;
protected:
    Config() {}
    Config(const Config &); // hide copy constructor
    Config& operator=(const Config &); // hide assign op
                                 // we leave just the declarations, so the compiler will warn us
                                 // if we try to use those two functions by accident

    static Config* m_instance;
};

static QObject *singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    Config *instance = Config::instance();
    return instance;
}

#endif // CONFIG_H
