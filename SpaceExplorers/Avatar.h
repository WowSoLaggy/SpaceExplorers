#pragma once

#include "Actions.h"
#include "Container.h"
#include "Object.h"

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

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();

  void interact(Action i_action = Action::Default, ObjectPtr io_object = nullptr,
                ObjectPtr i_tool = nullptr, const Sdk::Vector2I& i_where = Sdk::Vector2I::zero());

  Container& getInventory() { return d_inventory; }
  const Container& getInventory() const { return d_inventory; }

private:
  int d_maxSpeed = 256;

  int d_moveXSign = 0;
  int d_moveYSign = 0;

  Container d_inventory;

  void updateMovement(double i_dt);
  void updateMoveAnimation();

public:
  virtual void writeTo(std::ostream& io_stream) const override;
  virtual void readFrom(std::istream& io_stream) override;
};

using AvatarPtr = std::shared_ptr<Avatar>;
