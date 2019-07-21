#pragma once

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Vector.h>


class Cursor
{
public:
  Cursor(const Dx::IResourceController& i_resourceController);

  void setTexture(const std::string& i_textureName);
  void setPosition(Sdk::Vector2 i_position);
  void movePosition(const Sdk::Vector2& i_offset);

  void show();
  void hide();

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

private:
  const Dx::IResourceController& d_resourceController;
  bool d_visible = false;
  Dx::Sprite d_sprite{};
};
