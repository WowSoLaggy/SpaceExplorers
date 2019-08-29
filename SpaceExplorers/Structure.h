#pragma once

#include "Actions.h"
#include "Prototypes.h"
#include "Thing.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Vector.h>

#include <iostream>
#include <memory>


class Structure : public Thing
{
public:
  Structure(
    Dx::IResourceController& i_resourceController,
    const StructurePrototype& i_prototype,
    Sdk::Vector2I i_coordsTile);

  const StructurePrototype& getPrototype() const { return d_prototype; }

  virtual void update(double i_dt);
  virtual void render(Dx::IRenderer2d& i_renderer) const;

  virtual bool isStructure() const { return true; }

  virtual void interact(Action i_action = Action::Default);
  virtual bool isPassable() const;

  virtual bool checkAlpha(Sdk::Vector2I i_coords) const;

  bool isTransparent() const;
  const Sdk::Vector2I& getCoords() const;
  const Sdk::Vector2I& getCoordsTile() const;

  void resetColor();
  void setColor(Sdk::Vector4F i_color);

protected:
  Dx::IResourceController& d_resourceController;
  const StructurePrototype& d_prototype;
  std::shared_ptr<Dx::Sprite> d_sprite;
  Sdk::Vector2I d_coords;
  Sdk::Vector2I d_coordsTile;

public:
  virtual void writeTo(std::ostream& io_stream) const;
  virtual void readFrom(std::istream& io_stream);
};
