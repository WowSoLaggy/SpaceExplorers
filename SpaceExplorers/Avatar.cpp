#include "stdafx.h"
#include "Avatar.h"

#include "Utils.h"
#include "World.h"

#include <LaggySdk/Contracts.h>


Avatar::Avatar(
  Dx::IResourceController& i_resourceController,
  World& i_world, const ObjectPrototype& i_prototype)
  : Object(i_resourceController, i_world, i_prototype)
{
}


void Avatar::update(double i_dt)
{
  updateMovement(i_dt);
  Object::update(i_dt);
}


void Avatar::moveLeft()
{
  d_moveXSign -= 1;
}

void Avatar::moveRight()
{
  d_moveXSign += 1;
}

void Avatar::moveUp()
{
  d_moveYSign -= 1;
}

void Avatar::moveDown()
{
  d_moveYSign += 1;
}


void Avatar::updateMovement(double i_dt)
{
  auto isPassable = [&](const Sdk::Vector2I& i_tileCoords) -> bool
  {
    if (const auto* pTile = d_world.getTile(i_tileCoords))
    {
      if (const auto topStructure = pTile->getTopStructure())
        return topStructure->isPassable();
    }
    return true;
  };
  auto canMove = [&](Sdk::RectI i_curRect, const Sdk::Vector2I& i_diff) -> bool
  {
    i_curRect.shrink(2);

    std::unordered_set<Sdk::Vector2I, Sdk::Vector2_hash> tilesToCheck;
    tilesToCheck.insert(worldToTile(i_curRect.topLeft() + i_diff));
    tilesToCheck.insert(worldToTile(i_curRect.topRight() + i_diff));
    tilesToCheck.insert(worldToTile(i_curRect.bottomLeft() + i_diff));
    tilesToCheck.insert(worldToTile(i_curRect.bottomRight() + i_diff));

    return std::all_of(tilesToCheck.cbegin(), tilesToCheck.cend(), [&](const Sdk::Vector2I& i_tileCoords) {
      return isPassable(i_tileCoords);
    });

    return true;
  };

  if (d_moveXSign != 0)
  {
    Sdk::Vector2I diff;
    diff.x = (int)(d_maxSpeed * i_dt * d_moveXSign);

    if (canMove(d_rect, diff))
      setPosition(getPosition() + diff);
  }

  if (d_moveYSign != 0)
  {
    Sdk::Vector2I diff;
    diff.y = (int)(d_maxSpeed * i_dt * d_moveYSign);

    if (canMove(d_rect, diff))
      setPosition(getPosition() + diff);
  }

  if (d_moveXSign != 0 || d_moveYSign != 0)
  {
    updateMoveAnimation();
    d_moveXSign = 0;
    d_moveYSign = 0;
  }
}

void Avatar::updateMoveAnimation()
{
  if (d_moveXSign < 0)
    d_sprite.playAnimation("Left");
  else if (d_moveXSign > 0)
    d_sprite.playAnimation("Right");
  else if (d_moveYSign < 0)
    d_sprite.playAnimation("Up");
  else if (d_moveYSign > 0)
    d_sprite.playAnimation("Down");
}


void Avatar::interact(Action i_action /*= Action::Default*/, ObjectPtr io_object /*= nullptr*/,
                      ObjectPtr i_tool /*= nullptr*/, const Sdk::Vector2I& i_where /*= Sdk::Vector2I::zero()*/)
{
  CONTRACT_EXPECT(!i_tool || !i_tool->isAvatar());

  if (i_action == Action::Pickup)
  {
    if (io_object && !io_object->isAvatar() && d_inventory.tryAddObject(io_object))
      d_world.deleteObject(io_object);
  }
  else if (i_action == Action::Drop)
  {
    auto newObj = d_world.createObjectAt(i_tool->getPrototype(), i_where, i_tool->getName());
    newObj->setQuantity(1);

    if (i_tool->getQuantity() <= 1)
    {
      auto indexOpt = d_inventory.getObjectIndex(i_tool);
      CONTRACT_ASSERT(indexOpt);
      d_inventory.resetItem(*indexOpt);
    }
    else
      i_tool->addQuantity(-1);
  }
}
