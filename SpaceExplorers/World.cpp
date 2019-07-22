#include "stdafx.h"
#include "World.h"

void World::update(double i_dt)
{
  for (auto&[_, tile] : d_tilesMap)
    tile.update(i_dt);

  for (auto& item : d_objects)
    item->update(i_dt);
}

void World::render(Dx::IRenderer2d& i_renderer) const
{
  for (auto&[_, tile] : d_tilesMap)
    tile.render(i_renderer);

  for (const auto& item : d_objects)
    item->render(i_renderer);
}
