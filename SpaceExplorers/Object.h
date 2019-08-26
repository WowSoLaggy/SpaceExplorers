#pragma once

#include "Fwd.h"
#include "Thing.h"

#include <LaggyDx/AnimatedSprite.h>
#include <LaggyDx/LaggyDxFwd.h>
#include <LaggySdk/Rect.h>
#include <LaggySdk/Vector.h>

#include <iostream>
#include <memory>
#include <string>


class Object : public Thing
{
public:
  Object(
    Dx::IResourceController& i_resourceController,
    World& i_world, const ObjectPrototype& i_prototype);

  const ObjectPrototype& getPrototype() const { return d_prototype; }
  const Dx::AnimatedSprite& getSprite() const { return d_sprite; }

  virtual void update(double i_dt);
  virtual void render(Dx::IRenderer2d& i_renderer) const;

  virtual bool isObject() const { return true; }
 
  bool canBeStackedWith(ObjectPtr i_object) const;

  void setPosition(Sdk::Vector2I i_position);
  const Sdk::Vector2I& getPosition() const { return d_position; }

  void setName(std::string i_name) { d_name = std::move(i_name); }
  const std::string& getName() const { return d_name; }

  const Sdk::RectI& getRect() const { return d_rect; }
  bool checkAlpha(Sdk::Vector2I i_coords) const;

  void setQuantity(int i_quantity);
  void addQuantity(int i_delta);
  int getQuantity() const;

protected:
  Dx::IResourceController& d_resourceController;
  World& d_world;
  const ObjectPrototype& d_prototype;
  Dx::AnimatedSprite d_sprite;
  Sdk::Vector2I d_position;
  Sdk::RectI d_rect;

  std::string d_name;
  int d_quantity = 1;

public:
  virtual void writeTo(std::ostream& io_stream) const;
  virtual void readFrom(std::istream& io_stream);
};
