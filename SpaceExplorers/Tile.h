#pragma once

#include "Structure.h"

#include <LaggyDx/LaggyDxFwd.h>


class Tile
{
public:
  void update(double i_dt);
  int render(Dx::IRenderer2d& i_renderer) const;

  void setPanelling(StructurePtr i_panneling);
  void setFloor(StructurePtr i_floor);
  void setWall(StructurePtr i_wall);

private:
  StructurePtr d_panelling;
  StructurePtr d_floor;
  StructurePtr d_wall;
};
