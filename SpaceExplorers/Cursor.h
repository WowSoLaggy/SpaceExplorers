#pragma once

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Vector.h>


class Cursor
{
public:
  Cursor(Dx::IResourceController& i_resourceController);

  void setTexture(const std::string& i_textureName);
  void setPosition(Sdk::Vector2I i_position);
  const Sdk::Vector2I& getPosition() const;
  void movePosition(const Sdk::Vector2I& i_offset);

  void show();
  void hide();

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

private:
  Dx::IResourceController& d_resourceController;
  bool d_visible = false;
  Dx::Sprite d_sprite{};
};
