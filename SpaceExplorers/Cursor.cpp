#include "stdafx.h"
#include "Cursor.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>


Cursor::Cursor(const Dx::IResourceController& i_resourceController)
  : d_resourceController(i_resourceController)
{
}


void Cursor::setTexture(const std::string& i_textureName)
{
  d_sprite.setTexture(&d_resourceController.getTextureResource(i_textureName));
}

void Cursor::setPosition(Sdk::Vector2I i_position)
{
  d_sprite.setPosition(std::move(i_position));
}

const Sdk::Vector2I& Cursor::getPosition() const
{
  return d_sprite.getPosition();
}

void Cursor::movePosition(const Sdk::Vector2I& i_offset)
{
  const auto position = d_sprite.getPosition();
  d_sprite.setPosition(position + i_offset * 2);
}


void Cursor::show()
{
  d_visible = true;
}

void Cursor::hide()
{
  d_visible = false;
}


void Cursor::update(double i_dt)
{
}

void Cursor::render(Dx::IRenderer2d& i_renderer) const
{
  if (d_visible)
    i_renderer.renderSprite(d_sprite);
}
