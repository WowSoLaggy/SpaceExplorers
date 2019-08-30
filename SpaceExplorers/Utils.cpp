#include "stdafx.h"
#include "Utils.h"

#include "Camera.h"
#include "SettingsProvider.h"


Sdk::Vector2I screenToWorld(Sdk::Vector2I i_coords, const Camera& i_camera)
{
  i_coords += i_camera.getTopLeftPoint();
  return i_coords;
}

Sdk::Vector2I worldToScreen(Sdk::Vector2I i_coords, const Camera& i_camera)
{
  i_coords -= i_camera.getTopLeftPoint();
  return i_coords;
}


Sdk::Vector2I worldToTile(Sdk::Vector2I i_coords)
{
  const int tileSize = SettingsProvider::getDefaultInternalSettings().tileSize;
  auto getTileCoord = [&](int i_coord) -> int
  {
    return std::lround(std::floor((double)i_coord / tileSize));
  };

  return { getTileCoord(i_coords.x), getTileCoord(i_coords.y) };
}

Sdk::Vector2I screenToTile(Sdk::Vector2I i_coords, const Camera& i_camera)
{
  return worldToTile(screenToWorld(i_coords, i_camera));
}

Sdk::Vector2I tileToScreen(Sdk::Vector2I i_coords)
{
  return i_coords * SettingsProvider::getDefaultInternalSettings().tileSize;
}
