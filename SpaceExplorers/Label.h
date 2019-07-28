#pragma once

#include "IGuiControl.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggySdk/Vector.h>

#include <string>


class Label : public IGuiControl
{
public:
  Label(const Dx::IResourceController& i_resourceController);

  void update(double i_dt) override;
  void render(Dx::IRenderer2d& i_renderer) const override;

  void setText(std::string i_text) { d_text = std::move(i_text); }
  void setPosition(Sdk::Vector2I i_position) { d_position = std::move(i_position); }

private:
  const Dx::IFontResource& d_fontResource;

  std::string d_text;
  Sdk::Vector2I d_position;
};
