#include "gem.h"

#include "gemrenderer.h"

Gem::Gem(QObject *parent) :
    AbstractGeometry(parent)
{
    m_renderer = new GemRenderer();
}

Gem::~Gem()
{

}

void Gem::synchronize()
{

}
