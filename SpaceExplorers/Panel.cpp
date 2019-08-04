#include "stdafx.h"
#include "Panel.h"

#include <LaggyDx/IResourceController.h>
#include <LaggyDx/IRenderer2d.h>


Panel::Panel(const Dx::IResourceController& i_resourceController)
  : d_resourceController(i_resourceController)
{
}


void Panel::setTexture(const std::string& i_textureName)
{
  d_sprite.setTexture(&d_resourceController.getTextureResource(i_textureName));
}

void Panel::setPosition(Sdk::Vector2I i_position)
{
  d_sprite.setPosition(std::move(i_position));
}


Sdk::Vector2I Panel::getSize() const
{
  return d_sprite.getSize();
}

void Panel::setSize(Sdk::Vector2I i_size)
{
  d_sprite.setSize(std::move(i_size));
}


void Panel::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.renderSprite(d_sprite);
}
