#pragma once

#include <LaggyDx/LaggyDxFwd.h>

#include <memory>


class Object
{
public:
  void update(double i_dt);
  int render(Dx::IRenderer2d& i_renderer) const;
};

using ObjectPtr = std::shared_ptr<Object>;
