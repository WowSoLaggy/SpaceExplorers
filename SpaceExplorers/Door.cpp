#include "stdafx.h"
#include "Door.h"

#include <LaggyDx/AnimatedSprite.h>
#include <LaggySdk/Contracts.h>


Door::Door(
  const Dx::IResourceController& i_resourceController,
  const StructurePrototype& i_prototype,
  Sdk::Vector2I i_coordsTile)
  : Structure(i_resourceController, i_prototype, i_coordsTile)
{
  d_sprite = std::make_shared<Dx::AnimatedSprite>();
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
