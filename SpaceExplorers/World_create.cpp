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

  const int Lattices = 3;
  for (int y = -Lattices; y < Lattices; ++y)
  {
    for (int x = -Lattices; x < Lattices; ++x)
      tile(x, y).panelling = create(Prototypes::Lattice(), { x, y });
  }

  const int Floors = 2;
  for (int y = -Floors; y < Floors; ++y)
  {
    for (int x = -Floors; x < Floors; ++x)
      tile(x, y).floor = create(Prototypes::Floor(), { x, y });
  }

  const int Walls = 2;
  for (int y = -Floors; y < Floors; ++y)
  {
    for (int x = -Floors; x < Floors; ++x)
    {
      if (x == -Floors || x == Floors - 1 || y == -Floors || y == Floors - 1)
        tile(x, y).wall = create(Prototypes::Wall(), { x, y });
    }
  }

  ///

  return world;
}
