#include "gem.h"

#include "gemrenderer.h"

#include <ctime>
#include <cstdlib>

Gem::Gem(QObject *parent) :
    AbstractGeometry(parent)
{
    srand(static_cast <unsigned> (time(0)));
    float x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2.f - 1.f;
    float y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2.f - 1.f;
    float z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 2.f - 1.f;
    m_position.setX(x);
    m_position.setY(y);
    m_position.setZ(z);
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
}

void Gem::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
}
