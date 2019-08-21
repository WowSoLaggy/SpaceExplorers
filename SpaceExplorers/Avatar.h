#pragma once

#include "Object.h"


class Avatar : public Object
{
public:
  Avatar(
    Dx::IResourceController& i_resourceController,
    const World& i_world, const ObjectPrototype& i_prototype);

  virtual void update(double i_dt) override;

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();

private:
  const int MoveSpeed = 256;

private:
  int d_moveXSign = 0;
  int d_moveYSign = 0;

  void updateMovement(double i_dt);
  void updateMoveAnimation();
};

using AvatarPtr = std::shared_ptr<Avatar>;
