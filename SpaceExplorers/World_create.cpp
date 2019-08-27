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

  auto create = [&](int x, int y, const std::string& i_protoName) -> StructurePtr {
    return world->createStructureAt(PrototypesCollection::getStructure(i_protoName), { x, y });
  };

  auto toDoor = [](StructurePtr i_structure) -> DoorPtr {
    return std::dynamic_pointer_cast<Door>(i_structure);
  };

  auto createObject = [&](const std::string& i_protoName) -> ObjectPtr {
    const auto& proto = PrototypesCollection::getObject(i_protoName);
    auto objectPtr = std::make_shared<Object>(i_resourceController, *world, proto);
    return objectPtr;
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
  toDoor(create(0, -2, "Door"))->setState(Door::State::Open);
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


  auto tom = world->createAvatarAt(PrototypesCollection::getObject("Man"), { -32, -32 }, "Tom");
  tom->getInventory().setSize(8);
  tom->getInventory().setItem(2, createObject("Welder"));

  world->createObjectAt(PrototypesCollection::getObject("Rods"), { -64, -64 });
  world->createObjectAt(PrototypesCollection::getObject("Rods"), { -134, -64 });
  world->createObjectAt(PrototypesCollection::getObject("Rods"), { -64, -134 });
  world->createObjectAt(PrototypesCollection::getObject("Rods"), { -134, -134 });

  world->createObjectAt(PrototypesCollection::getObject("Welder"), { 128, 128 });


  ///

  return world;
}
