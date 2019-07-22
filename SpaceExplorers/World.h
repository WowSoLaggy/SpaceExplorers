#pragma once

#include "Object.h"
#include "Structure.h"
#include "Tile.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggySdk/Vector.h>

#include <unordered_map>
#include <vector>


class World
{
public:
  static std::unique_ptr<World> createTestWorld(const Dx::IResourceController& i_resourceController);

public:
  void update(double i_dt);
  int render(Dx::IRenderer2d& i_renderer) const;

private:
  std::unordered_map<Sdk::Vector2I, Tile, Sdk::Vector2_hash> d_tilesMap;

  std::vector<ObjectPtr> d_objects;
};
