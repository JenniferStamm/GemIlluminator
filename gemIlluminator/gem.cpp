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

    m_renderer->setPosition(m_position);
    m_renderer->setRotation(m_rotation);
}

void Gem::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}
