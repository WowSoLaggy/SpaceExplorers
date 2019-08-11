#pragma once

#include "Prototypes.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Vector.h>

#include <iostream>
#include <memory>


class Object
{
public:
  Object(
    Dx::IResourceController& i_resourceController,
    const ObjectPrototype& i_prototype,
    Sdk::Vector2I i_position);

  virtual void update(double i_dt);
  virtual void render(Dx::IRenderer2d& i_renderer) const;

protected:
  Dx::IResourceController& d_resourceController;
  const ObjectPrototype& d_prototype;
  std::shared_ptr<Dx::Sprite> d_sprite;
  Sdk::Vector2I d_position;

public:
  virtual void writeTo(std::ostream& io_stream) const;
  virtual void readFrom(std::istream& io_stream);
};

using ObjectPtr = std::shared_ptr<Object>;
