#include "stdafx.h"
#include "World.h"

#include "Door.h"
#include "PrototypesCollection.h"
#include "Structure.h"

#include <LaggyDx/IResourceController.h>


std::unique_ptr<World> World::createEmptyWorld(
  Sdk::Vector2I i_backgroundSize,
  Dx::IResourceController& i_resourceController)
{
  auto world = std::make_unique<World>(i_resourceController);
  world->setBackground("Space.png", std::move(i_backgroundSize));
  return world;
}


std::unique_ptr<World> World::createTestWorld(
  Sdk::Vector2I i_backgroundSize,
  Dx::IResourceController& i_resourceController)
{
  auto world = createEmptyWorld(i_backgroundSize, i_resourceController);

  ///

  auto create = [&](int x, int y, const std::string& i_protoName) {
    world->createStructureAt(PrototypesCollection::getStructure(i_protoName), { x, y });
  };

  ///

  const int Lattices = 5;
  for (int y = -Lattices; y < Lattices; ++y)
  {
    for (int x = -Lattices; x < Lattices; ++x)
      create(x, y, "Lattice");
  }

  for (int y = -3; y < 3; ++y)
  {
    for (int x = -8; x < -5; ++x)
      create(x, y, "Lattice");
  }

  //

  const int Floors = 4;
  for (int y = -Floors; y < Floors; ++y)
  {
    for (int x = -Floors; x < Floors; ++x)
      create(x, y, "Floor");
  }

  for (int y = -2; y < 2; ++y)
  {
    for (int x = -7; x < -4; ++x)
      create(x, y, "Floor");
  }

  //

  const int Walls = 4;
  for (int y = -Floors; y < Floors; ++y)
  {
    for (int x = -Floors; x < Floors; ++x)
    {
      if (x == -Floors || x == Floors - 1 || y == -Floors || y == Floors - 1)
        create(x, y, "Wall");
    }
  }


  create(0, -3, "Wall");
  create(0, -2, "Door");
  create(0, -1, "Wall");
  create(0, 0, "Wall");
  create(1, 0, "Wall");
  create(2, 0, "Wall");
  create(0, 1, "Door");
  create(0, 2, "Wall");

  create(-5, -2, "Wall");
  create(-6, -2, "Wall");
  create(-7, -2, "Wall");
  create(-7, -1, "Wall");
  create(-7, 0, "Door");
  create(-7, 1, "Wall");
  create(-6, 1, "Wall");
  create(-5, 1, "Wall");
  create(-4, -1, "Door");


  ///


  {
    Object avatar(i_resourceController, PrototypesCollection::getObject("Man"));
    avatar.setPosition({ -32, -32 });
    world->d_objects.push_back(std::move(avatar));
  }


  ///

  return world;
}
