#pragma once

#include "Container.h"
#include "Object.h"


class Avatar : public Object
{
public:
  Avatar(
    Dx::IResourceController& i_resourceController,
    World& i_world, const ObjectPrototype& i_prototype);

  virtual void update(double i_dt) override;

  virtual bool isAvatar() const override { return true; }

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();

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
