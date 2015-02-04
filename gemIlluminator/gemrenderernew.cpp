#include "gemrenderernew.h"

#include <QList>

#include "abstractgem.h"

GemRendererNew::GemRendererNew() :
    m_isInitialized(false)
{
}

GemRendererNew::~GemRendererNew()
{

}

void GemRendererNew::paint()
{
    if (!m_isInitialized) {
        initialize();
    }
}

void GemRendererNew::initialize()
{
    m_isInitialized = true;
}
