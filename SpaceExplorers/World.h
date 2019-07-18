#pragma once

#include "Object.h"
#include "Structure.h"

#include <LaggyDx/LaggyDxFwd.h>

#include <vector>


class World
{
public:
  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

private:
  std::vector<Structure> d_structures;
  std::vector<Object> d_objects;
};
