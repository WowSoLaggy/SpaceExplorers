#pragma once

#include "IGuiControl.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Vector.h>

#include <string>


class Panel : public IGuiControl
{
public:
  Panel(Dx::IResourceController& i_resourceController);

  virtual void render(Dx::IRenderer2d& i_renderer) const override;

  void setTexture(const std::string& i_textureName);
  void setPosition(Sdk::Vector2I i_position);

  virtual Sdk::Vector2I getSize() const override;
  void setSize(Sdk::Vector2I i_size);

private:
  Dx::IResourceController& d_resourceController;

  Dx::Sprite d_sprite;
};
