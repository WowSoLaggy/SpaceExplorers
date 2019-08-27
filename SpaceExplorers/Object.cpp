#include "stdafx.h"
#include "Object.h"

#include "Prototypes.h"
#include "SettingsProvider.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>
#include <LaggySdk/Contracts.h>

#include <unordered_set>


Object::Object(
  Dx::IResourceController& i_resourceController,
  World& i_world, const ObjectPrototype& i_prototype)
  : d_resourceController(i_resourceController)
  , d_world(i_world)
  , d_prototype(i_prototype)
{
  d_sprite.setTexture(&d_resourceController.getTextureResource(d_prototype.textureFileName));
  
  setPosition({ 0, 0 });
}


void Object::update(double i_dt)
{
  d_sprite.update(i_dt);
}

void Object::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.renderSprite(d_sprite);
}


void Object::setPosition(Sdk::Vector2I i_position)
{
  d_position = std::move(i_position);

  const auto& size = d_sprite.getSize();
  const auto topLeft = d_position - size / 2;
  d_rect = { topLeft, topLeft + size };

  d_sprite.setPosition(topLeft);
}


bool Object::canBeStackedWith(ObjectPtr i_object) const
{
  return 
    i_object &&
    d_prototype.name == i_object->d_prototype.name &&
    d_name == i_object->d_name;
}


void Object::setQuantity(int i_quantity)
{
  CONTRACT_EXPECT(i_quantity <= 1 || d_prototype.isStackable);
  d_quantity = i_quantity;
  CONTRACT_ENSURE(d_quantity >= 0);
}

void Object::addQuantity(int i_delta)
{
  d_quantity += i_delta;
  CONTRACT_ENSURE(d_quantity <= 1 || d_prototype.isStackable);
  CONTRACT_ENSURE(d_quantity >= 0);
}

int Object::getQuantity() const
{
  return d_quantity;
}


bool Object::checkAlpha(Sdk::Vector2I i_coords) const
{
  return d_sprite.getTexture()->checkAlpha(i_coords, d_sprite.getFrame());
}
