#include "stdafx.h"
#include "Cursor.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>


Cursor::Cursor(const Dx::IResourceController& i_resourceController)
  : d_resourceController(i_resourceController)
  , d_visible(false)
  , d_textureId(Dx::ResourceIdEmpty)
{
}


void Cursor::setTexture(const std::string& i_textureName)
{
  d_textureId = d_resourceController.getResourceId(i_textureName);

  // TODO: ae Either provide me a reliable method to get the texture size,
  // or don't ask me to provide you its size in the 'render' method!
  //d_textureSize = d_resourceController.getSize(d_textureId);
  d_textureSize = { 25, 25 };
}

void Cursor::setPosition(Sdk::Vector2 i_position)
{
  d_position = std::move(i_position);
}


void Cursor::show()
{
  d_visible = true;
}

void Cursor::hide()
{
  d_visible = false;
}


void Cursor::update(double i_dt)
{
}

void Cursor::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.renderTexture(d_textureId, d_position, d_textureSize);
}
