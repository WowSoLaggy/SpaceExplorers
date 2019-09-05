#include "stdafx.h"
#include "Door.h"

#include "SettingsProvider.h"

#include <LaggyDx/AnimatedSprite.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>
#include <LaggySdk/Contracts.h>


Door::Door(
  Dx::IResourceController& i_resourceController,
  const StructurePrototype& i_prototype,
  Sdk::Vector2I i_coordsTile)
  : Structure(i_resourceController, i_prototype, i_coordsTile)
{
}


void Door::update(double i_dt)
{
  d_sprite.update(i_dt);
  if (d_state == State::Opening && !d_sprite.isPlaying())
    d_state = State::Open;
  else if (d_state == State::Closing && !d_sprite.isPlaying())
    d_state = State::Closed;
}


Action Door::getDefaultAction() const
{
  if (d_state == State::Closed)
    return Action::Open;
  else if (d_state == State::Open)
    return Action::Close;

  return Action::None;
}

void Door::interact(Action i_action)
{
  if (i_action == Action::None)
    return;
  else if (i_action == Action::Default)
  {
    auto defaultAction = getDefaultAction();
    CONTRACT_EXPECT(defaultAction != Action::Default);
    interact(defaultAction);
    return;
  }
  else if (i_action == Action::Open)
  {
    if (d_state == State::Closed)
    {
      d_sprite.playAnimation("Open");
      d_state = State::Opening;
    }
  }
  else if (i_action == Action::Close)
  {
    if (d_state == State::Open)
    {
      d_sprite.playAnimation("Close");
      d_state = State::Closing;
    }
  }
}


bool Door::isPassable() const
{
  return d_state == State::Open || d_state == State::Closing;
}


void Door::setState(State i_state)
{
  d_state = i_state;

  if (d_state == State::Open)
  {
    d_sprite.playAnimation("Open");
    d_sprite.setFrameEnd();
  }
  else if (d_state == State::Closed)
  {
    d_sprite.playAnimation("Close");
    d_sprite.setFrameEnd();
  }
  else if (d_state == State::Opening)
    d_sprite.playAnimation("Open");
  else if (d_state == State::Closing)
    d_sprite.playAnimation("Close");

}
