#ifndef FILEIO_H
#define FILEIO_H

#include <QObject>

/**
 * @brief The FileIO class provides platform independent file reading for resource files used.
 * @detail The file that should be read is specified by name and FileIO will load it from platform dependent location.
 */
class FileIO : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    /**
     * @brief Creates a new FileIO
     * @param parent QObject-parent
     */
    explicit FileIO(QObject *parent = 0);
    ~FileIO();

    /**
      * @brief Reads previous specified file.
      * @return Read file as QString.
      */
    Q_INVOKABLE QString read();
    /**
     * @brief Sets the source file by name. The source is read calling read()
     * @param source The name of file that should be read.
     */
    void setSource(const QString& source);
    /**
     * @brief The name of file that is read by read().
     * @return
     */
    QString source();
    /**
      * @brief Writes specified data into source()
      * @param data The data that will be written into source()
      */
    Q_INVOKABLE bool write(const QString& data);

signals:
    /**
     * @brief This signal will be emitted if an error occurs
     * @param msg The error message describing the error
     */
    void error(const QString& msg);
    /**
     * @brief This signal is emitted if the source() is changed.
     * @param source The new source
     */
    void sourceChanged(const QString& source);

protected:
    QString m_source;
};

#endif // FILEIO_H
