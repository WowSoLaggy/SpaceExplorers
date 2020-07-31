#include "stdafx.h"
#include "Structure.h"

#include "BehaviorModelFactory.h"
#include "ContainerModel.h"
#include "DoorModel.h"
#include "PowerSourceModel.h"
#include "PowerLoadModel.h"
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
  if (d_sprite.getRotation() > 0.01)
    i_coords.rotate(-d_sprite.getRotation(), { d_sprite.getSize().x / 2 - 1, d_sprite.getSize().y / 2 - 1 });

  return d_sprite.getTexture()->checkAlpha(i_coords, d_sprite.getFrame());
}


bool Structure::isDoor() const
{
  return getBehaviorModel<DoorModel>().get() != nullptr;
}

bool Structure::isContainer() const
{
  return getBehaviorModel<ContainerModel>() != nullptr;
}

bool Structure::isPowerSource() const
{
  return getBehaviorModel<PowerSourceModel>() != nullptr;
}

bool Structure::isPowerLoad() const
{
  return getBehaviorModel<PowerLoadModel>() != nullptr;
}


void Structure::setOrientation(Sdk::Side i_orientation)
{
  static const std::unordered_map<Sdk::Side, double> SideAngles{
    { Sdk::Side::Up, Sdk::degToRad<double>(180) },
    { Sdk::Side::Left, Sdk::degToRad<double>(90) },
    { Sdk::Side::Down, Sdk::degToRad<double>(0) },
    { Sdk::Side::Right, Sdk::degToRad<double>(270) },
  };

  d_orientation = i_orientation;

  getSprite().setRotation(SideAngles.at(d_orientation));
}

Sdk::Side Structure::getOrientation() const
{
  return d_orientation;
}


bool Structure::canHaveAttachment(AttachmentPosition i_position) const
{
  return d_attachments.find(i_position) != d_attachments.cend();
}

bool Structure::canHaveAttachment(AttachmentGeneralPosition i_position) const
{
  if (i_position == AttachmentGeneralPosition::None)
    return false;
  else if (i_position == AttachmentGeneralPosition::Center)
    return canHaveAttachment(AttachmentPosition::Center);
  else if (i_position == AttachmentGeneralPosition::Side)
  {
    return
      canHaveAttachment(AttachmentPosition::Up) &&
      canHaveAttachment(AttachmentPosition::Left) &&
      canHaveAttachment(AttachmentPosition::Down) &&
      canHaveAttachment(AttachmentPosition::Right);
  }

  CONTRACT_ASSERT(false);
}

StructurePtr Structure::getAttachment(AttachmentPosition i_position) const
{
  CONTRACT_EXPECT(canHaveAttachment(i_position));
  return d_attachments.at(i_position);
}

bool Structure::hasAttachment(AttachmentPosition i_position) const
{
  CONTRACT_EXPECT(canHaveAttachment(i_position));
  return d_attachments.at(i_position).get() != nullptr;
}

void Structure::setAttachment(AttachmentPosition i_position, StructurePtr i_attachment)
{
  CONTRACT_EXPECT(canHaveAttachment(i_position));
  d_attachments.at(i_position) = i_attachment;
}
