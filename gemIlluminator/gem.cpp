#include "gem.h"

#include "gemrenderer.h"

Gem::Gem(QObject *parent) :
    AbstractGeometry(parent)
{
}

Gem::~Gem()
{
}

void Gem::synchronize()
{
    //renderer has to been created in correct thread
    if (!m_renderer){
        m_renderer = new GemRenderer();
    }
}

void Gem::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}
