#include "stdafx.h"
#include "Object.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>


Object::Object(
  Dx::IResourceController& i_resourceController,
  const ObjectPrototype& i_prototype,
  Sdk::Vector2I i_position)
  : d_resourceController(i_resourceController)
  , d_prototype(i_prototype)
  , d_position(std::move(i_position))
{
  d_sprite = std::make_shared<Dx::Sprite>();

  d_sprite->setTexture(&d_resourceController.getTextureResource(d_prototype.textureFileName));
  d_sprite->setPosition(d_position);
}


void Object::update(double i_dt)
{
  // nop
}

void Object::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.renderSprite(*d_sprite);
}
