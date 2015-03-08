#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>

class FileIO : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit FileIO(QObject *parent = 0);
    ~FileIO();

    Q_INVOKABLE QString read();
    void setSource(const QString& source);
    QString source();
    Q_INVOKABLE bool write(const QString& data);

signals:
    void error(const QString& msg);
    void sourceChanged(const QString& source);

protected:
    QString m_source;
};

#endif // FILEIO_H
