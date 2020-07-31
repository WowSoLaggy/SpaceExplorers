#pragma once

#include "Actions.h"
#include "Container.h"
#include "Fwd.h"
#include "Object.h"
#include "WalkModes.h"

#include <LaggySdk/Vector.h>


class Avatar : public Object
{
public:
  Avatar(
    Dx::IResourceController& i_resourceController,
    World& i_world, const ObjectPrototype& i_prototype);

  virtual void update(double i_dt) override;

  virtual bool isObject() const { return false; }
  virtual bool isAvatar() const { return true; }

  virtual void processEvent(const Sdk::IEvent& i_event) override;

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();

  void interact(Action i_action, ThingPtr io_object,
                ObjectPtr i_tool, const Sdk::Vector2I& i_where);

  bool isInteracting() const;

  bool isInspectingContainer() const { return d_inspectedContainer != nullptr; }
  void startInspectingContainer(StructurePtr i_structure);
  void stopInspectingContainer();

  void startBuilding(BuildContextPtr i_buildContext);
  void stopBuilding();
  bool isBuilding() const { return !!d_buildContext; }

  Container& getInventory() { return d_inventory; }
  const Container& getInventory() const { return d_inventory; }

  void setRun();
  void setWalk();
  bool isRun() const;
  bool isWalk() const;

private:
  Sdk::Vector2I d_curSpeed;
  int d_interactionDistSq = 96 * 96;

  WalkMode d_walkMode = WalkMode::Run;
  const int RunSpeed = 256;
  const int WalkSpeed = 128;

  int d_moveXSign = 0;
  int d_moveYSign = 0;

  Container d_inventory;

  StructurePtr d_inspectedContainer;

  BuildContextPtr d_buildContext;
  void updateBuild(double i_dt);
  void finishBuild();

  void updateMovement(double i_dt);
  void updateMoveAnimation();

  int getMaxSpeed();

public:
  virtual void writeTo(std::ostream& io_stream) const override;
  virtual void readFrom(std::istream& io_stream) override;
};

using AvatarPtr = std::shared_ptr<Avatar>;
