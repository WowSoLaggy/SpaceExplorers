#include "stdafx.h"
#include "Structure.h"

#include "BehaviorModelFactory.h"
#include "ContainerModel.h"
#include "DoorModel.h"
#include "SettingsProvider.h"

#include <LaggyDx/ImageDescription.h>
#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>
#include <LaggySdk/Contracts.h>


Structure::Structure(
  Dx::IResourceController& i_resourceController,
  World& i_world, const StructurePrototype& i_prototype,
  Sdk::Vector2I i_coordsTile)
  : d_resourceController(i_resourceController)
  , d_world(i_world)
  , d_prototype(i_prototype)
  , d_coordsTile(std::move(i_coordsTile))
{
  d_sprite.setTexture(&d_resourceController.getTextureResource(d_prototype.textureFileName));
  
  auto tileSize = SettingsProvider::getDefaultInternalSettings().tileSize;
  d_sprite.setPosition(d_coordsTile * tileSize);

  d_coords = d_coordsTile * tileSize;
  d_coords += { tileSize / 2, tileSize / 2 };

  d_behaviorModel = BehaviorModelFactory::get(*this, d_world);
  CONTRACT_ENSURE(d_behaviorModel);
}


void Structure::update(double i_dt)
{
  CONTRACT_EXPECT(d_behaviorModel);
  d_behaviorModel->update(i_dt);
}

void Structure::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.renderSprite(d_sprite);
}


void Structure::interact(const Action i_action)
{
  CONTRACT_EXPECT(d_behaviorModel);
  d_behaviorModel->interact(i_action);
}

bool Structure::isPassable() const
{
  CONTRACT_EXPECT(d_behaviorModel);
  return d_behaviorModel->isPassable();
}

bool Structure::hasAtmosphere() const
{
  CONTRACT_EXPECT(d_behaviorModel);
  return d_behaviorModel->hasAtmosphere();
}

bool Structure::isLeak() const
{
  CONTRACT_EXPECT(d_behaviorModel);
  return d_behaviorModel->isLeak();
}

bool Structure::isSupport() const
{
  CONTRACT_EXPECT(d_behaviorModel);
  return d_behaviorModel->isSupport();
}


bool Structure::isTransparent() const
{
  return d_sprite.getTexture()->getDescription().alpha;
}

const Sdk::Vector2I& Structure::getCoords() const
{
  return d_coords;
}

const Sdk::Vector2I& Structure::getCoordsTile() const
{
  return d_coordsTile;
}


void Structure::resetColor()
{
  d_sprite.setColor(Sdk::Vector4F::identity());
}

void Structure::setColor(Sdk::Vector4F i_color)
{
  d_sprite.setColor(std::move(i_color));
}


bool Structure::checkAlpha(Sdk::Vector2I i_coords) const
{
  return d_sprite.getTexture()->checkAlpha(i_coords, d_sprite.getFrame());
}


bool Structure::isDoor() const
{
  return !!std::dynamic_pointer_cast<DoorModel>(d_behaviorModel);
}

bool Structure::isContainer() const
{
  return !!std::dynamic_pointer_cast<ContainerModel>(d_behaviorModel);
}


std::shared_ptr<DoorModel> Structure::getDoorModel() const
{
  return std::dynamic_pointer_cast<DoorModel>(d_behaviorModel);
}

std::shared_ptr<ContainerModel> Structure::getContainerModel() const
{
  return std::dynamic_pointer_cast<ContainerModel>(d_behaviorModel);
}
