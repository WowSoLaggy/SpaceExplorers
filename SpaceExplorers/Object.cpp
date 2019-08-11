#include "stdafx.h"
#include "Object.h"

#include "Prototypes.h"
#include "SettingsProvider.h"
#include "Utils.h"
#include "World.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>

#include <unordered_set>


Object::Object(
  Dx::IResourceController& i_resourceController,
  const World& i_world, const ObjectPrototype& i_prototype)
  : d_resourceController(i_resourceController)
  , d_world(i_world)
  , d_prototype(i_prototype)
{
  d_sprite.setTexture(&d_resourceController.getTextureResource(d_prototype.textureFileName));
  
  setPosition({ 0, 0 });
}


void Object::update(double i_dt)
{
  updateMovement(i_dt);
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


void Object::moveLeft()
{
  d_moveXSign -= 1;
}

void Object::moveRight()
{
  d_moveXSign += 1;
}

void Object::moveUp()
{
  d_moveYSign -= 1;
}

void Object::moveDown()
{
  d_moveYSign += 1;
}


void Object::updateMovement(double i_dt)
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
    diff.x = (int)(MoveSpeed * i_dt * d_moveXSign);

    if (canMove(d_rect, diff))
      setPosition(getPosition() + diff);
  }

  if (d_moveYSign != 0)
  {
    Sdk::Vector2I diff;
    diff.y = (int)(MoveSpeed * i_dt * d_moveYSign);

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

void Object::updateMoveAnimation()
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
