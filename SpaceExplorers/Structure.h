#pragma once

#include "Actions.h"
#include "StructurePrototype.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Vector.h>

#include <iostream>
#include <memory>


class Structure
{
public:
  Structure(
    Dx::IResourceController& i_resourceController,
    const StructurePrototype& i_prototype,
    Sdk::Vector2I i_coordsTile);

  virtual void update(double i_dt);
  virtual void render(Dx::IRenderer2d& i_renderer) const;

  virtual void interact(Actions i_action = Actions::Default);

  bool isTransparent() const;
  const Sdk::Vector2I& getCoordsTile() const;

  void resetColor();
  void setColor(Sdk::Vector4F i_color);

protected:
  const StructurePrototype& d_prototype;
  std::shared_ptr<Dx::Sprite> d_sprite;
  const Sdk::Vector2I d_coordsTile;

protected:
  virtual std::ostream& write(std::ostream& io_stream) const;
  friend std::ostream& operator<<(std::ostream& io_stream, const Structure& i_structure);
};

using StructurePtr = std::shared_ptr<Structure>;
