#pragma once

#include "Object.h"
#include "Structure.h"

#include <LaggyDx/LaggyDxFwd.h>

#include <vector>


class World
{
public:
  static std::unique_ptr<World> createTestWorld(const Dx::IResourceController& i_resourceController);

public:
  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

private:
  std::vector<StructurePtr> d_structures;
  std::vector<ObjectPtr> d_objects;
};
