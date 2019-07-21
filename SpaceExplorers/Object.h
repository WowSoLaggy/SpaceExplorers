#pragma once

#include <LaggyDx/LaggyDxFwd.h>


class Object
{
public:
  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;
};
