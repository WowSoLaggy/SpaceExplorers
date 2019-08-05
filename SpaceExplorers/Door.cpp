#include "stdafx.h"
#include "Door.h"

#include "SettingsProvider.h"

#include <LaggyDx/AnimatedSprite.h>
#include <LaggyDx/IResourceController.h>
#include <LaggySdk/Contracts.h>


Door::Door(
  Dx::IResourceController& i_resourceController,
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
  if (d_state == State::Opening && !getAnimatedSprite().isPlaying())
    d_state = State::Open;
  else if (d_state == State::Closing && !getAnimatedSprite().isPlaying())
    d_state = State::Closed;
}


Dx::AnimatedSprite& Door::getAnimatedSprite()
{
  return dynamic_cast<Dx::AnimatedSprite&>(*d_sprite);
}


Actions Door::getDefaultAction() const
{
  if (d_state == State::Closed)
    return Actions::Open;
  else if (d_state == State::Open)
    return Actions::Close;

  return Actions::None;
}

void Door::interact(Actions i_action)
{
  if (i_action == Actions::None)
    return;
  else if (i_action == Actions::Default)
  {
    auto defaultAction = getDefaultAction();
    CONTRACT_EXPECT(defaultAction != Actions::Default);
    interact(defaultAction);
    return;
  }
  else if (i_action == Actions::Open)
  {
    if (d_state == State::Closed)
    {
      getAnimatedSprite().playAnimation("Open");
      d_state = State::Opening;
    }
  }
  else if (i_action == Actions::Close)
  {
    if (d_state == State::Open)
    {
      getAnimatedSprite().playAnimation("Close");
      d_state = State::Closing;
    }
  }
}
