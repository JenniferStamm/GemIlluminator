#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

class Config : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
public:
    explicit Config(QObject *parent = 0);
    ~Config();

    Q_INVOKABLE QString read();
    Q_INVOKABLE bool write(const QString& data);

    QString source();

public slots:
    void setSource(const QString& source);

signals:
    void sourceChanged(const QString& source);
    void error(const QString& msg);

private:
    QString m_source;
};

#endif // CONFIG_H
