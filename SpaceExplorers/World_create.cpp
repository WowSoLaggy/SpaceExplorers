#include "stdafx.h"
#include "World.h"

#include "Prototypes.h"
#include "Structure.h"

#include <LaggyDx/IResourceController.h>


std::unique_ptr<World> World::createTestWorld(const Dx::IResourceController& i_resourceController)
{
  auto world = std::make_unique<World>();

  ///

  auto create = [&](const StructurePrototype& i_prototype, Sdk::Vector2I i_tileCoords)
  {
    return std::make_shared<Structure>(i_resourceController, i_prototype, std::move(i_tileCoords));
  };

  auto tile = [&](int x, int y) -> Tile&
  {
    return world->d_tilesMap[{x, y}];
  };

  ///

  for (int y = -5; y < 5; ++y)
  {
    for (int x = -5; x < 5; ++x)
      tile(x, y).panelling = create(Prototypes::Lattice(), { x, y });
  }

  ///

  return world;
}
