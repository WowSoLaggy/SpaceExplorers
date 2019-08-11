#pragma once

#include "Prototypes.h"

#include <LaggyDx/AnimatedSprite.h>
#include <LaggyDx/LaggyDxFwd.h>
#include <LaggySdk/Rect.h>
#include <LaggySdk/Vector.h>

#include <iostream>
#include <memory>
#include <string>


class Object
{
public:
  Object(
    Dx::IResourceController& i_resourceController,
    const ObjectPrototype& i_prototype);

  virtual void update(double i_dt);
  virtual void render(Dx::IRenderer2d& i_renderer) const;

  void setPosition(Sdk::Vector2I i_position);
  const Sdk::Vector2I& getPosition() const { return d_position; }

  void setName(std::string i_name) { d_name = std::move(i_name); }
  const std::string& getName() const { return d_name; }

  const Sdk::RectI& getRect() const { return d_rect; }

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();

private:
  const int MoveSpeed = 256;

private:
  Dx::IResourceController& d_resourceController;
  const ObjectPrototype& d_prototype;
  Dx::AnimatedSprite d_sprite;
  Sdk::Vector2I d_position;
  Sdk::RectI d_rect;

  std::string d_name;

  int d_moveXSign = 0;
  int d_moveYSign = 0;

  void updateMoveAnimation();

public:
  virtual void writeTo(std::ostream& io_stream) const;
  virtual void readFrom(std::istream& io_stream);
};

using ObjectPtr = std::shared_ptr<Object>;
