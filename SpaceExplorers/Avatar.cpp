#include "stdafx.h"
#include "Avatar.h"

#include "Utils.h"
#include "World.h"

#include <LaggyDx/ImageDescription.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>
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
  auto canMove = [&](Sdk::RectI i_curRect, const Sdk::Vector2I& i_diff) -> bool
  {
    i_curRect.move(i_diff);
    i_curRect.shrink(2);
    
    return !d_world.checkCollision(i_curRect);
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


void Avatar::interact(Action i_action /*= Action::Default*/, ThingPtr io_object /*= nullptr*/,
                      ObjectPtr i_tool /*= nullptr*/, const Sdk::Vector2I& i_where /*= Sdk::Vector2I::zero()*/)
{
  CONTRACT_EXPECT(!i_tool || !i_tool->isAvatar());

  if (i_action == Action::Default)
  {
    if (!io_object || i_tool)
      return;

    if (io_object->isStructure())
    {
      auto structure = std::dynamic_pointer_cast<Structure>(io_object);
      CONTRACT_ASSERT(structure);

      if ((structure->getCoords() - d_position).lengthSq() <= d_interactionDistSq)
        structure->interact(i_action);
    }
  }
  else if (i_action == Action::Pickup)
  {
    if (!io_object || !io_object->isObject())
      return;

    auto obj = std::dynamic_pointer_cast<Object>(io_object);
    CONTRACT_ASSERT(obj);

    if ((obj->getPosition() - d_position).lengthSq() > d_interactionDistSq)
      return;
    
    if (d_inventory.tryAddObject(obj))
      d_world.deleteObject(obj);
  }
  else if (i_action == Action::Drop)
  {
    if ((i_where - d_position).lengthSq() > d_interactionDistSq)
    {
      // Can't throw too far away
      return;
    }

    const auto& proto = i_tool->getPrototype();

    {
      const auto& texture = d_resourceController.getTextureResource(proto.textureFileName);
      const Sdk::Vector2I size { texture.getDescription().width, texture.getDescription().height };
      const auto topLeft = i_where - size / 2;
      Sdk::RectI rect = { topLeft, topLeft + size };
      rect.shrink(4);
      if (d_world.checkCollision(rect))
      {
        // No place for the object
        return;
      }
    }

    auto newObj = d_world.createObjectAt(proto, i_where, i_tool->getName());
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
