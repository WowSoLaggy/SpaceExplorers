#include "stdafx.h"
#include "DoorModel.h"

#include "Structure.h"
#include "World.h"

#include <LaggyDx/AnimatedSprite.h>
#include <LaggySdk/Contracts.h>


DoorModel::DoorModel(Structure& io_structure, World& io_world)
  : BehaviorModelBase(io_structure, io_world)
{
}


void DoorModel::update(double i_dt)
{
  auto& sprite = d_structure.getSprite();

  sprite.update(i_dt);
  if (d_state == State::Opening && !sprite.isPlaying())
    d_state = State::Open;
  else if (d_state == State::Closing && !sprite.isPlaying())
    onClose();
}


void DoorModel::interact(Action i_action)
{
  auto& sprite = d_structure.getSprite();

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
      sprite.playAnimation("Open");
      d_state = State::Opening;
    }
  }
  else if (i_action == Action::Close)
  {
    if (d_state == State::Open)
    {
      sprite.playAnimation("Close");
      d_state = State::Closing;
    }
  }
}


bool DoorModel::isPassable() const
{
  return d_state != State::Closed;
}

bool DoorModel::hasAtmosphere() const
{
  return d_state != State::Closed;
}


void DoorModel::setState(State i_state)
{
  d_state = i_state;

  auto& sprite = d_structure.getSprite();

  if (d_state == State::Open)
  {
    sprite.playAnimation("Open");
    sprite.setFrameEnd();
  }
  else if (d_state == State::Closed)
  {
    sprite.playAnimation("Close");
    sprite.setFrameEnd();
  }
  else if (d_state == State::Opening)
    sprite.playAnimation("Open");
  else if (d_state == State::Closing)
    sprite.playAnimation("Close");

}


Action DoorModel::getDefaultAction() const
{
  if (d_state == State::Closed)
    return Action::Open;
  else if (d_state == State::Open)
    return Action::Close;

  return Action::None;
}


void DoorModel::onClose()
{
  d_state = State::Closed;

  if (d_world.checkIntersectWithAnyObject(d_structure.getSprite().getRect()))
    interact(Action::Open);
}


void DoorModel::writeTo(std::ostream& io_stream) const
{
  Sdk::write(io_stream, d_state);
}

void DoorModel::readFrom(std::istream& io_stream, Dx::IResourceController& i_resouceContoller, World& i_world)
{
  Sdk::read(io_stream, d_state);
}
