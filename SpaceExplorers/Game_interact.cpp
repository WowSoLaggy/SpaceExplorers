#include "stdafx.h"
#include "Game.h"


void Game::tryInteract()
{
  const auto& mousePos = d_gui.getCursor().getPosition();
  const auto tileCoords = screenToTile(mousePos);

  auto* tile = d_world->getTile(tileCoords);
  if (!tile)
    return;

  auto structure = tile->getTopStructure();
  if (!structure)
    return;

  structure->interact();
}
