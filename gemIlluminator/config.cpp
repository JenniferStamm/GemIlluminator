#include "config.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMutex>
#include <QTextStream>

Config* Config::m_instance = 0;

QString Config::read()
{
    if (m_source.isEmpty()) {
        emit error("Source is empty.");
        return QString();
    }

    QFile *file = new QFile();

#ifdef __ANDROID__
    file->setFileName("assets:/" + m_source);
#endif
#ifdef _WIN32
    file->setFileName(QApplication::applicationDirPath() + "/assets/" + m_source);
#endif

    QString fileContent;
    if (file->open(QIODevice::ReadOnly)) {
        QString line;
        QTextStream t(file);
        do {
            line = t.readLine();
            fileContent += line;
        } while (!line.isNull());

        file->close();
        delete file;
    } else {
        emit error("Unable to open the file.");
        return QString();
    }
    return fileContent;
}

bool Config::write(const QString& data)
{
    if (m_source.isEmpty())
        return false;

    QFile *file = new QFile();

#ifdef __ANDROID__
    file->setFileName("assets:/" + m_source);
#endif
#ifdef _WIN32
    file->setFileName(QApplication::applicationDirPath() + "/assets/" + m_source);
#endif

    if (!file->open(QFile::WriteOnly | QFile::Truncate))
        return false;

    QTextStream out(file);
    out << data;

    file->close();
    delete file;

    return true;
}

void Config::setAxisRange(int &axisRange)
{
    m_axisRange = axisRange;
    emit axisRangeChanged();
}

QString Config::source()
{
    return m_source;
}

void Config::setSource(const QString &source)
{
    m_source = source;
}
QString Config::envMap() const
{
    return m_envMap;
}

void Config::setEnvMap(const QString &envMap)
{
    m_envMap = envMap;
    emit envMapChanged();
}


int Config::axisRange()
{
    return m_axisRange;
}

void Config::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_instance;
    m_instance = 0;
    mutex.unlock();
}

Config *Config::instance()
{
    static QMutex mutex;
    if (!m_instance)
    {
        mutex.lock();

        if (!m_instance)
            m_instance = new Config;

        mutex.unlock();
    }

    return m_instance;
}
