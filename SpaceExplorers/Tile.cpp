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
  if (d_wall)
    d_wall->render(i_renderer);
  else if (d_floor)
    d_floor->render(i_renderer);
  else if (d_panelling)
    d_panelling->render(i_renderer);

  return 1;
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
