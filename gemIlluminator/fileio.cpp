#include "fileio.h"

#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

FileIO::FileIO(QObject *parent) : QObject(parent)
{

}

QString FileIO::read()
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

bool FileIO::write(const QString& data)
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

QString FileIO::source()
{
    return m_source;
}

void FileIO::setSource(const QString &source)
{
    m_source = source;
}

FileIO::~FileIO()
{

}
