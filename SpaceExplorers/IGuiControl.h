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

  void setName(std::string i_name) { d_name = std::move(i_name); }
  const std::string& getName() const { return d_name; }

private:
  std::string d_name = "";
};

using IGuiControlPtr = std::shared_ptr<IGuiControl>;
