#include "soundmanager.h"

#include <QMutex>

Soundmanager* Soundmanager::m_instance = 0;

void Soundmanager::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_instance;
    m_instance = 0;
    mutex.unlock();
}

Soundmanager *Soundmanager::instance()
{
    static QMutex mutex;
    if (!m_instance)
    {
        mutex.lock();

        if (!m_instance)
            m_instance = new Soundmanager;

        mutex.unlock();
    }

    return m_instance;
}

