#include "stdafx.h"
#include "Game.h"

#include "Inventory.h"

#include <LaggySdk/Contracts.h>


void Game::tryInteract()
{
  if (auto obj = d_world->getObjectAt(cursorToWorld()))
  {
    if (!obj->isAvatar() && d_avatar)
      tryPickup(*d_avatar, obj);
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

void Game::tryPickup(Avatar& io_avatar, ObjectPtr io_object)
{
  CONTRACT_EXPECT(io_object);
  d_world->deleteObject(io_object);

  auto inventory = std::dynamic_pointer_cast<Inventory>(d_gui.getControl("Inventory"));
  CONTRACT_ASSERT(inventory);

  std::optional<int> index;
  if (io_object->getPrototype().isStackable)
  {
    if (index = inventory->getObjectIndex(io_object))
    {
      inventory->getItem(*index)->addQuantity(1);
      return;
    }
  }

  index = inventory->getFreeSlot();
  if (!index)
  {
    // No free space in inventory!
    return;
  }

  inventory->setItem(*index, io_object);
}
