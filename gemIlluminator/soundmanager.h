#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QSound>
#include <QQmlEngine>

class Soundmanager : public QObject
{
    Q_OBJECT
public:
    static Soundmanager* instance();
    static void drop();

signals:

public slots:

protected:
    Soundmanager() {}
    Soundmanager(const Soundmanager &); // hide copy constructor
    Soundmanager& operator=(const Soundmanager &); // hide assign op
                                 // we leave just the declarations, so the compiler will warn us
                                 // if we try to use those two functions by accident

    static Soundmanager* m_instance;
};

static QObject *singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    Soundmanager *instance = Soundmanager::instance();
    return instance;
}

#endif // SOUNDMANAGER_H
