#include "stdafx.h"
#include "World.h"


void World::update(double i_dt)
{
  for (auto&[_, tile] : d_tilesMap)
  {
    if (tile.wall)
      tile.wall->update(i_dt);
    if (tile.floor)
      tile.floor->update(i_dt);
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
    if (tile.wall)
      tile.wall->render(i_renderer);
    else if (tile.floor)
      tile.floor->render(i_renderer);
    else if (tile.panelling)
      tile.panelling->render(i_renderer);
  }

  for (const auto& item : d_objects)
    item->render(i_renderer);
}
