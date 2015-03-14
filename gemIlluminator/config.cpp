#include "config.h"

#include <QMutex>

Config* Config::m_instance = 0;

Config::Config()
{
}

void Config::setAxisRange(int &axisRange)
{
    m_axisRange = axisRange;
    emit axisRangeChanged();
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

void Config::setMaxGemSize(float maxGemSize)
{
    if (m_maxGemSize == maxGemSize) {
        return;
    }
    m_maxGemSize = maxGemSize;
    emit maxGemSizeChanged();
}

void Config::setMinGemSize(float minGemSize)
{
    if (m_minGemSize == minGemSize) {
        return;
    }
    m_minGemSize = minGemSize;
    emit minGemSizeChanged();
}

Config::~Config()
{

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
    if (!m_instance) {
        mutex.lock();

        if (!m_instance) {
            m_instance = new Config;
        }

        mutex.unlock();
    }

    return m_instance;
}

float Config::maxGemSize() const
{
    return m_maxGemSize;
}

float Config::minGemSize() const
{
    return m_minGemSize;
}

QObject *configSingletontypeProvider(QQmlEngine * /*engine*/, QJSEngine * /*scriptEngine*/)
{
    return Config::instance();
}
