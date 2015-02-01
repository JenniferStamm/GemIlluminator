#include "config.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>

Config::Config(QObject *parent) : QObject(parent)
{

}

QString Config::read()
{
    if (m_source.isEmpty()) {
        emit error("Source is empty.");
        return QString();
    }

    QFile file;
    file.setFileName(QApplication::applicationDirPath() + "/" + m_source);
    qDebug() << QApplication::applicationDirPath() + "/" + m_source;
    QString fileContent;
    if (file.open(QIODevice::ReadOnly)) {
        QString line;
        QTextStream t(&file);
        do {
            line = t.readLine();
            fileContent += line;
         } while (!line.isNull());

        file.close();
    } else {
        qDebug() << "test";
        emit error("Unable to open the file.");
        return QString();
    }
    return fileContent;
}

bool Config::write(const QString& data)
{
    if (m_source.isEmpty())
        return false;

    QFile file(m_source);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return false;

    QTextStream out(&file);
    out << data;

    file.close();

    return true;
}

QString Config::source()
{
    return m_source;
}

void Config::setSource(const QString &source)
{
    m_source = source;
}

Config::~Config()
{

}

