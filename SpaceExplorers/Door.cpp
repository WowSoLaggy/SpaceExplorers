#include "stdafx.h"
#include "Door.h"

#include "SettingsProvider.h"

#include <LaggyDx/AnimatedSprite.h>
#include <LaggyDx/IResourceController.h>
#include <LaggySdk/Contracts.h>


Door::Door(
  const Dx::IResourceController& i_resourceController,
  const StructurePrototype& i_prototype,
  Sdk::Vector2I i_coordsTile)
  : Structure(i_resourceController, i_prototype, i_coordsTile)
{
  // TODO: ae Remove this duplication from the base class ctor
  d_sprite = std::make_shared<Dx::AnimatedSprite>();

  d_sprite->setTexture(&i_resourceController.getTextureResource(d_prototype.textureFileName));
  d_sprite->setPosition(i_coordsTile * SettingsProvider::getDefaultInternalSettings().tileSize);
}


void Door::update(double i_dt)
{
  getAnimatedSprite().update(i_dt);
}


Dx::AnimatedSprite& Door::getAnimatedSprite()
{
  return dynamic_cast<Dx::AnimatedSprite&>(*d_sprite);
}


Actions Door::getDefaultAction() const
{
  return Actions::Open;
}

void Door::interact(Actions i_action)
{
  if (i_action == Actions::None)
    return;

  if (i_action == Actions::Default)
  {
    auto defaultAction = getDefaultAction();
    CONTRACT_EXPECT(defaultAction != Actions::Default);
    interact(defaultAction);
  }

  getAnimatedSprite().playAnimation("Open");
}
