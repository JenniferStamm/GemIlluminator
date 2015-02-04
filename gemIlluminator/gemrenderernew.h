#ifndef GEMRENDERERNEW_H
#define GEMRENDERERNEW_H

template<typename T> class QList;

class AbstractGem;

class GemRendererNew
{
public:
    GemRendererNew();
    ~GemRendererNew();

    void paint();

protected:
    void initialize();

protected:
    bool m_isInitialized;
};

#endif // GEMRENDERERNEW_H
