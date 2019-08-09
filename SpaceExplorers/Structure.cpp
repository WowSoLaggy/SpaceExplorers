#include "stdafx.h"
#include "Structure.h"

#include "SettingsProvider.h"

#include <LaggyDx/ImageDescription.h>
#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>


Structure::Structure(
  Dx::IResourceController& i_resourceController,
  const StructurePrototype& i_prototype,
  Sdk::Vector2I i_coordsTile)
  : d_resourceController(i_resourceController)
  , d_prototype(i_prototype)
  , d_coordsTile(std::move(i_coordsTile))
{
  d_sprite = std::make_shared<Dx::Sprite>();

  d_sprite->setTexture(&d_resourceController.getTextureResource(d_prototype.textureFileName));
  d_sprite->setPosition(d_coordsTile * SettingsProvider::getDefaultInternalSettings().tileSize);
}


void Structure::update(double i_dt)
{
  // nop
}

void Structure::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.renderSprite(*d_sprite);
}


void Structure::interact(const Actions i_action)
{
  // nop
}


bool Structure::isTransparent() const
{
  return d_sprite->getTexture()->getDescription().alpha;
}

const Sdk::Vector2I& Structure::getCoordsTile() const
{
  return d_coordsTile;
}


void Structure::resetColor()
{
  d_sprite->setColor(Sdk::Vector4F::identity());
}

void Structure::setColor(Sdk::Vector4F i_color)
{
  d_sprite->setColor(std::move(i_color));
}
