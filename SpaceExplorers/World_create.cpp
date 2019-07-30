#include "stdafx.h"
#include "World.h"

#include "Door.h"
#include "Prototypes.h"
#include "Structure.h"

#include <LaggyDx/IResourceController.h>


std::unique_ptr<World> World::createTestWorld(
  Sdk::Vector2I i_backgroundSize,
  const Dx::IResourceController& i_resourceController)
{
  auto world = std::make_unique<World>(i_resourceController);
  world->setBackground("Space.png", std::move(i_backgroundSize));

  ///

  auto createLattice = [&](int x, int y) {
    world->createStructureAt(Prototypes::Lattice(), { x, y });
  };

  auto createFloor = [&](int x, int y) {
    world->createStructureAt(Prototypes::Floor(), { x, y });
  };

  auto createWall = [&](int x, int y) {
    world->createStructureAt(Prototypes::Wall(), { x, y });
  };

  auto createDoor = [&](int x, int y) {
    world->createStructureAt(Prototypes::Door(), { x, y });
  };

  ///

  const int Lattices = 5;
  for (int y = -Lattices; y < Lattices; ++y)
  {
    for (int x = -Lattices; x < Lattices; ++x)
      createLattice(x, y);
  }

  for (int y = -3; y < 3; ++y)
  {
    for (int x = -8; x < -5; ++x)
      createLattice(x, y);
  }

  //

  const int Floors = 4;
  for (int y = -Floors; y < Floors; ++y)
  {
    for (int x = -Floors; x < Floors; ++x)
      createFloor(x, y);
  }

  for (int y = -2; y < 2; ++y)
  {
    for (int x = -7; x < -4; ++x)
      createFloor(x, y);
  }

  //

  const int Walls = 4;
  for (int y = -Floors; y < Floors; ++y)
  {
    for (int x = -Floors; x < Floors; ++x)
    {
      if (x == -Floors || x == Floors - 1 || y == -Floors || y == Floors - 1)
        createWall(x, y);
    }
  }


  createWall(0, -3);
  createDoor(0, -2);
  createWall(0, -1);
  createWall(0, 0);
  createWall(1, 0);
  createWall(2, 0);
  createDoor(0, 1);
  createWall(0, 2);

  createWall(-5, -2);
  createWall(-6, -2);
  createWall(-7, -2);
  createWall(-7, -1);
  createDoor(-7, 0);
  createWall(-7, 1);
  createWall(-6, 1);
  createWall(-5, 1);
  createDoor(-4, -1);


  ///

  return world;
}
