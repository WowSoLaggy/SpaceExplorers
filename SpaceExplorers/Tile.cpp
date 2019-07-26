#include "stdafx.h"
#include "Tile.h"


void Tile::update(double i_dt)
{
  if (d_wall)
    d_wall->update(i_dt);
  if (d_floor)
    d_floor->update(i_dt);
  if (d_panelling)
    d_panelling->update(i_dt);
}

int Tile::render(Dx::IRenderer2d& i_renderer) const
{
  const std::vector<StructurePtr> renderList{ d_panelling, d_floor, d_wall };
  auto it = renderList.end() - 1;

  while (it != renderList.begin() && (*it == nullptr || (*it)->isTransparent()))
    --it;

  int drawnObjects = 0;
  for (; it != renderList.end(); ++it)
  {
    if (*it != nullptr)
    {
      (*it)->render(i_renderer);
      ++drawnObjects;
    }
  }

  return drawnObjects;
}


void Tile::setPanelling(StructurePtr i_panneling)
{
  d_panelling = i_panneling;
}

void Tile::setFloor(StructurePtr i_floor)
{
  d_floor = i_floor;
}

void Tile::setWall(StructurePtr i_wall)
{
  d_wall = i_wall;
}


StructurePtr Tile::getTopStructure()
{
  if (d_wall)
    return d_wall;
  else if (d_floor)
    return d_floor;
  else
    return d_panelling;
}

const StructurePtr Tile::getTopStructure() const
{
  if (d_wall)
    return d_wall;
  else if (d_floor)
    return d_floor;
  else
    return d_panelling;
}
