#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

#include "fileio.h"

class QJSEngine;
class QQmlEngine;

/**
 * @brief The Config class provides easy access to values read out of our config.json provided by ConfigView.qml.
 * @detail The config class is implemented as singleton, because these values are used in many various situations.
 * This class works together ConfigView.qml.
 */
class Config : public FileIO
{
    Q_OBJECT
    Q_PROPERTY(int axisRange READ axisRange WRITE setAxisRange NOTIFY axisRangeChanged)
    Q_PROPERTY(QString envMap READ envMap WRITE setEnvMap NOTIFY envMapChanged)
    Q_PROPERTY(float maxGemSize READ maxGemSize WRITE setMaxGemSize NOTIFY maxGemSizeChanged)
    Q_PROPERTY(float minGemSize READ minGemSize WRITE setMinGemSize NOTIFY minGemSizeChanged)

    Config(const Config &) = delete;
    Config& operator=(const Config &) = delete;
public:
    virtual ~Config();

    /**
     * @brief The axisRange describes how far the scene expands along all axis.
     * @return The value of axisRange
     * @see setAxisRange()
     */
    int axisRange();
    /**
     * @brief Drops current instance of our Config
     */
    static void drop();
    /**
     * @brief envMap is the name of our current environment. The name is the prefix of our environment texture names.
     * @return
     */
    QString envMap() const;
    /**
     * @brief The instance of our Config.
     * @return
     */
    static Config *instance();
    /**
     * @brief maxGemSize is the maximum size a gem could be.
     * @detail This is the same as maximum scale factor for a gem, because all gems are
     * in range between [-1;1] by default.
     * @return
     * @see setMaxGemSize()
     */
    float maxGemSize() const;
    /**
     * @brief minGemSize is the minimum size a gem could be.
     * @detail This is the same as minimum scale factor for a gem, because all gems are
     * in range between [-1;1] by default.
     * @return
     */
    float minGemSize() const;
    /**
     * @brief Sets value of axisRange();
     * @param The new value
     */
    void setAxisRange(int& axisRange);
    /**
     * @brief Sets value of envMap();
     * @param The new value
     */
    void setEnvMap(const QString &envMap);
    /**
     * @brief Sets value of maxGemSize();
     * @param The new value
     */
    void setMaxGemSize(float maxGemSize);
    /**
     * @brief Sets value of minGemSize();
     * @param The new value
     */
    void setMinGemSize(float minGemSize);

signals:
    void axisRangeChanged();
    void envMapChanged();
    void maxGemSizeChanged();
    void minGemSizeChanged();

protected:
    Config();

protected:
    int m_axisRange;
    QString m_envMap;
    static Config *m_instance;
    float m_maxGemSize;
    float m_minGemSize;
};

/**
 * @brief Callback function used to get the current instance of Config within QML.
 * @param engine Unused parameter required by callback.
 * @param scriptEngine Unused parameter required by callback.
 * @return Our instance of Config
 */
QObject *configSingletontypeProvider(QQmlEngine *engine, QJSEngine *scriptEngine);

#endif // CONFIG_H
