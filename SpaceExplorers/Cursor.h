#pragma once

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggySdk/Vector.h>


class Cursor
{
public:
  Cursor(const Dx::IResourceController& i_resourceController);

  void setTexture(const std::string& i_textureName);
  void setPosition(Sdk::Vector2 i_position);

  void show();
  void hide();

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

private:
  const Dx::IResourceController& d_resourceController;
  bool d_visible;
  Sdk::Vector2 d_position;
  Sdk::Vector2 d_textureSize;
  Dx::ResourceId d_textureId;
};
