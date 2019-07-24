#include "stdafx.h"
#include "World.h"

#include "SettingsProvider.h"


namespace
{
  Sdk::RectI getTileRect(const Sdk::Vector2I& i_coords)
  {
    const int tileSize = SettingsProvider::getDefaultInternalSettings().tileSize;

    const Sdk::Vector2I topLeft = i_coords * tileSize;
    const Sdk::Vector2I bottomRight = topLeft + Sdk::Vector2I{ tileSize, tileSize };

    return Sdk::RectI{ topLeft, bottomRight};
  }
} // anonymous NS

void World::update(double i_dt)
{
  for (auto&[_, tile] : d_tilesMap)
    tile.update(i_dt);

  for (auto& item : d_objects)
    item->update(i_dt);
}

int World::render(Dx::IRenderer2d& i_renderer, const Sdk::RectI& i_viewport) const
{
  int renderedThings = 0;

  for (const auto&[coords, tile] : d_tilesMap)
  {
    const auto tileRect = getTileRect(coords);
    if (i_viewport.intersectRect(tileRect))
      renderedThings += tile.render(i_renderer);
  }

  /*for (const auto& item : d_objects)
    item->render(i_renderer);*/

  return renderedThings;
}
