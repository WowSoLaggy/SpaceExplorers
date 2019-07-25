#pragma once

#include "Actions.h"
#include "StructurePrototype.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Vector.h>

#include <memory>


class Structure
{
public:
  Structure(
    const Dx::IResourceController& i_resourceController,
    const StructurePrototype& i_prototype,
    Sdk::Vector2I i_coordsTile);

  virtual void update(double i_dt);
  virtual void render(Dx::IRenderer2d& i_renderer) const;

  virtual void interact(Actions i_action);

protected:
  const StructurePrototype& d_prototype;
  Dx::Sprite d_sprite{};
};

using StructurePtr = std::shared_ptr<Structure>;
