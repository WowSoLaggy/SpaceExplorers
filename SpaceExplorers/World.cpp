#include "stdafx.h"
#include "World.h"


void World::update(double i_dt)
{
  for (auto&[_, tile] : d_tilesMap)
  {
    if (tile.panelling)
      tile.panelling->update(i_dt);
  }

  for (auto& item : d_objects)
    item->update(i_dt);
}

void World::render(Dx::IRenderer2d& i_renderer) const
{
  for (auto&[_, tile] : d_tilesMap)
  {
    if (tile.panelling)
      tile.panelling->render(i_renderer);
  }

  for (const auto& item : d_objects)
    item->render(i_renderer);
}
