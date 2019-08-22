#include "stdafx.h"
#include "Game.h"

#include "Actions.h"
#include "Inventory.h"

#include <LaggySdk/Contracts.h>


void Game::tryInteract()
{
  if (d_avatar)
  {
    if (auto obj = d_world->getObjectAt(cursorToWorld()))
    {
      auto selectedTool = getSelectedTool();
      Action action = selectedTool ? Action::None : Action::Pickup;
      d_avatar->interact(action, obj, selectedTool);
      return;
    }
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
