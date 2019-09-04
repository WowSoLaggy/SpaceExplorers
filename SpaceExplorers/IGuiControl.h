#pragma once

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/MouseKey.h>
#include <LaggySdk/Vector.h>


class IGuiControl
{
public:
  virtual ~IGuiControl() = default;

  virtual void update(double i_dt) { };
  virtual void render(Dx::IRenderer2d& i_renderer) const { };

  virtual Sdk::Vector2I getSize() const = 0;

  virtual bool onMouseClick(Dx::MouseKey i_button, const Sdk::Vector2I& i_mousePos) { return false; };
  virtual void onMouseRelease(Dx::MouseKey i_button, const Sdk::Vector2I& i_mousePos) { };
  virtual void onMouseMove(const Sdk::Vector2I& i_mousePos) { };

  void setName(std::string i_name) { d_name = std::move(i_name); }
  const std::string& getName() const { return d_name; }

private:
  std::string d_name = "";
};

using IGuiControlPtr = std::shared_ptr<IGuiControl>;
