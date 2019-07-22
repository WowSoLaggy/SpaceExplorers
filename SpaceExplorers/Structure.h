#pragma once

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

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

private:
  const StructurePrototype& d_prototype;
  Sdk::Vector2I d_coordsTile{};
  Dx::Sprite d_sprite{};
};

using StructurePtr = std::shared_ptr<Structure>;
