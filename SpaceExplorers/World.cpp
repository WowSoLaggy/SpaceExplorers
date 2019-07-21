#include "stdafx.h"
#include "World.h"


void World::update(double i_dt)
{
  auto process = [&](auto& i_input)
  {
    for (auto& item : i_input)
      item.update(i_dt);
  };

  process(d_structures);
  process(d_objects);
}

void World::render(Dx::IRenderer2d& i_renderer) const
{
  auto process = [&](const auto& i_input)
  {
    for (const auto& item : i_input)
      item.render(i_renderer);
  };

  process(d_structures);
  process(d_objects);
}
