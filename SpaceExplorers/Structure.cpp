#include "stdafx.h"
#include "Structure.h"

#include "SettingsProvider.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>


Structure::Structure(
  const Dx::IResourceController& i_resourceController,
  const StructurePrototype& i_prototype,
  Sdk::Vector2I i_coordsTile)
  : d_prototype(i_prototype)
{
  d_sprite.setTexture(&i_resourceController.getTextureResource(d_prototype.textureFileName));
  d_sprite.setPosition(i_coordsTile * SettingsProvider::getDefaultInternalSettings().tileSize);
}


void Structure::update(double i_dt)
{
  // nop
}

void Structure::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.renderSprite(d_sprite);
}


void Structure::interact(const Actions i_action)
{
  // nop
}
