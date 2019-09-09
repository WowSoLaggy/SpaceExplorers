#pragma once

#include "Actions.h"
#include "BehaviorModelBase.h"
#include "Fwd.h"
#include "Prototypes.h"
#include "Thing.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/AnimatedSprite.h>
#include <LaggySdk/EventHandler.h>
#include <LaggySdk/Vector.h>

#include <iostream>
#include <memory>


class Structure : public Thing, public Sdk::EventHandler
{
public:
  Structure(
    Dx::IResourceController& i_resourceController,
    World& i_world, const StructurePrototype& i_prototype,
    Sdk::Vector2I i_coordsTile);

  const StructurePrototype& getPrototype() const { return d_prototype; }
  std::shared_ptr<BehaviorModelBase> getBehaviorModel() { return d_behaviorModel; }

  Dx::AnimatedSprite& getSprite() { return d_sprite; }
  const Dx::AnimatedSprite& getSprite() const { return d_sprite; }

  virtual void update(double i_dt);
  virtual void render(Dx::IRenderer2d& i_renderer) const;

  virtual void interact(Action i_action = Action::Default);
  virtual bool isPassable() const;

  virtual bool checkAlpha(Sdk::Vector2I i_coords) const;

  virtual bool isStructure() const { return true; }

  bool isDoor() const;
  bool isContainer() const;

  std::shared_ptr<DoorModel> getDoorModel() const;
  std::shared_ptr<ContainerModel> getContainerModel() const;

  bool isTransparent() const;
  const Sdk::Vector2I& getCoords() const;
  const Sdk::Vector2I& getCoordsTile() const;

  void resetColor();
  void setColor(Sdk::Vector4F i_color);

  void setBuildTime(double i_buildTime) { d_buildTime = i_buildTime; }
  void addBuildTime(double i_addedBuildTime) { d_buildTime += i_addedBuildTime; }
  double getBuildTime() const { return d_buildTime; }

protected:
  Dx::IResourceController& d_resourceController;
  World& d_world;
  const StructurePrototype& d_prototype;
  Dx::AnimatedSprite d_sprite;
  Sdk::Vector2I d_coords;
  Sdk::Vector2I d_coordsTile;

  std::shared_ptr<BehaviorModelBase> d_behaviorModel;

  double d_buildTime = 0;

public:
  virtual void writeTo(std::ostream& io_stream) const;
  virtual void readFrom(std::istream& io_stream);
};
