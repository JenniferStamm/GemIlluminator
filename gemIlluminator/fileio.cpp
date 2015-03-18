#include "fileio.h"

#include <QApplication>
#include <QDebug>
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
    file->setFileName("./" + m_source);

    // Assets directory is read only copy the file if needed
    QFile *dfile = new QFile("assets:/" + m_source);

    if (dfile->exists() && !file->exists()) {

         dfile->copy("./" + m_source);
         QFile::setPermissions("./" + m_source, QFile::WriteOwner | QFile::WriteGroup | QFile::ReadOwner | QFile::ReadGroup);
    }

    dfile->close();
    delete dfile;
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
    file->setFileName("./" + m_source);

    // Assets directory is read only copy the file if needed
    QFile *dfile = new QFile("assets:/" + m_source);

    if (dfile->exists() && !file->exists()) {
         dfile->copy("./" + m_source);
         QFile::setPermissions("./" + m_source, QFile::WriteOwner | QFile::WriteGroup | QFile::ReadOwner | QFile::ReadGroup);
    }

    dfile->close();
    delete dfile;
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
    if (m_source == source) {
        return;
    }
    m_source = source;
    emit sourceChanged(source);
}

FileIO::~FileIO()
{
}

