#include "stdafx.h"
#include "Game.h"


void Game::tryInteract()
{
  if (auto obj = d_world->getObjectAt(cursorToWorld()))
  {
    if (!obj->isAvatar() && d_avatar)
      tryPickup(*d_avatar, *obj);
    return;
  }

  const auto tileCoords = cursorToTile();

  auto* tile = d_world->getTile(tileCoords);
  if (!tile)
    return;

  auto structure = tile->getTopStructure();
  if (!structure)
    return;

  structure->interact();
}

void Game::tryPickup(Avatar& io_avatar, Object& io_object)
{
  d_world->deleteObject(io_object);
}
