#pragma once

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggySdk/Vector.h>


class IGuiControl
{
public:
  virtual ~IGuiControl() = default;

  virtual void update(double i_dt) { };
  virtual void render(Dx::IRenderer2d& i_renderer) const { };

  virtual Sdk::Vector2I getSize() const = 0;
};

using IGuiControlPtr = std::shared_ptr<IGuiControl>;
