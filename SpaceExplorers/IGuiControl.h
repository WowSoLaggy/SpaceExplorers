#pragma once

#include <LaggyDx/LaggyDxFwd.h>


class IGuiControl
{
public:
  virtual ~IGuiControl() = default;

  virtual void update(double i_dt) { };
  virtual void render(Dx::IRenderer2d& i_renderer) const { };
};

using IGuiControlPtr = std::shared_ptr<IGuiControl>;
