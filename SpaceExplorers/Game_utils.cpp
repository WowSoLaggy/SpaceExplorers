#include "stdafx.h"
#include "Game.h"

#include "SettingsProvider.h"


Sdk::Vector2I Game::screenToWorld(Sdk::Vector2I i_coords) const
{
  i_coords += d_camera.getTopLeftPoint();
  return i_coords;
}

Sdk::Vector2I Game::worldToTile(Sdk::Vector2I i_coords) const
{
  const int tileSize = SettingsProvider::getDefaultInternalSettings().tileSize;
  auto getTileCoord = [&](int i_coord) -> int
  {
    return std::lround(std::floor((double)i_coord / tileSize));
  };
  
  return { getTileCoord(i_coords.x), getTileCoord(i_coords.y) };
}

Sdk::Vector2I Game::screenToTile(Sdk::Vector2I i_coords) const
{
  return worldToTile(screenToWorld(i_coords));
}

Sdk::Vector2I Game::tileToScreen(Sdk::Vector2I i_coords) const
{
  return i_coords * SettingsProvider::getDefaultInternalSettings().tileSize;
}
