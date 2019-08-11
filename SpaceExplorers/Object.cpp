#include "stdafx.h"
#include "Object.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>


Object::Object(
  Dx::IResourceController& i_resourceController,
  const ObjectPrototype& i_prototype)
  : d_resourceController(i_resourceController)
  , d_prototype(i_prototype)
{
  d_sprite = std::make_shared<Dx::Sprite>();
  d_sprite->setTexture(&d_resourceController.getTextureResource(d_prototype.textureFileName));
  
  setPosition({ 0, 0 });
}


void Object::update(double i_dt)
{
  // nop
}

void Object::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.renderSprite(*d_sprite);
}


void Object::setPosition(Sdk::Vector2I i_position)
{
  d_position = std::move(i_position);

  const auto& size = d_sprite->getSize();
  const auto topLeft = d_position - size / 2;
  d_rect = { topLeft, topLeft + size };

  d_sprite->setPosition(topLeft);
}
