#pragma once

#include <LaggyDx/LaggyDxFwd.h>


class IGuiControl
{
public:
  virtual ~IGuiControl() = default;

  virtual void update(double i_dt) = 0;
  virtual void render(Dx::IRenderer2d& i_renderer) const = 0;
};

using IGuiControlPtr = std::shared_ptr<IGuiControl>;
