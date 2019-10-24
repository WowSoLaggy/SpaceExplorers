#include "stdafx.h"
#include "World.h"

#include "ContainerModel.h"
#include "DoorModel.h"
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

  auto toDoor = [](StructurePtr i_structure) -> std::shared_ptr<DoorModel> {
    return std::dynamic_pointer_cast<DoorModel>(i_structure->getBehaviorModel());
  };

  auto createObject = [&](const std::string& i_protoName) -> ObjectPtr {
    const auto& proto = PrototypesCollection::getObject(i_protoName);
    auto objectPtr = std::make_shared<Object>(i_resourceController, *world, proto);
    objectPtr->setName(i_protoName);
    return objectPtr;
  };

  ///

  const int Lattices = 5;
  for (int y = -Lattices; y < Lattices; ++y)
  {
    for (int x = -Lattices; x < Lattices; ++x)
    {
      if (x == -Lattices || x == Lattices - 1 || y == -Lattices || y == Lattices - 1)
        create(x, y, "Lattice");
      else
        create(x, y, "Plating");
    }
  }

  for (int y = -3; y < 3; ++y)
  {
    for (int x = -8; x < -4; ++x)
    {
      if (x == -8 || x == -4 - 1 || y == -3 || y == 3 - 1)
        create(x, y, "Lattice");
    }
  }

  for (int y = -2; y < 2; ++y)
  {
    for (int x = -7; x < -4; ++x)
      create(x, y, "Plating");
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
  toDoor(create(0, -2, "Door"))->setState(DoorModel::State::Open);
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
  toDoor(create(-7, 0, "Door"))->setState(DoorModel::State::Open);
  create(-7, 1, "Wall");
  create(-6, 1, "Wall");
  create(-5, 1, "Wall");
  create(-4, -1, "Door");


  ///


  {
    auto crate = create(-6, -1, "Crate");
    auto& container = crate->getContainerModel()->getContainer();

    for (int i = 0; i < 3; ++i)
    {
      auto rods = createObject("Rods");
      rods->setQuantity(40);
      container.setItem(i, rods);

      auto metal = createObject("Metal");
      metal->setQuantity(40);
      container.setItem(i + 4, metal);
    }
  }


  ///


  {
    auto tom = world->createAvatarAt(PrototypesCollection::getObject("Man"), { -352, 32 }, "Tom");
    tom->getInventory().setSize(8);
    tom->getInventory().setItem(0, createObject("Crowbar"));
    tom->getInventory().setItem(1, createObject("Welder"));
    tom->getInventory().setItem(2, createObject("CuttingTorch"));

    auto rods = createObject("Rods");
    rods->setQuantity(40);
    tom->getInventory().setItem(3, rods);

    auto floorTiles = createObject("FloorTile");
    floorTiles->setQuantity(40);
    tom->getInventory().setItem(4, floorTiles);

    auto metal = createObject("Metal");
    metal->setQuantity(40);
    tom->getInventory().setItem(5, metal);
  }

  world->createObjectAt(PrototypesCollection::getObject("Rods"), { -64, -64 }, "Rods")->setQuantity(40);
  world->createObjectAt(PrototypesCollection::getObject("Rods"), { -128, -64 }, "Rods")->setQuantity(40);
  world->createObjectAt(PrototypesCollection::getObject("Rods"), { -64, -128 }, "Rods")->setQuantity(40);
  world->createObjectAt(PrototypesCollection::getObject("Rods"), { -128, -128 }, "Rods")->setQuantity(40);

  world->createObjectAt(PrototypesCollection::getObject("Metal"), { -64, -96 }, "Metal")->setQuantity(40);
  world->createObjectAt(PrototypesCollection::getObject("Metal"), { -128, -96 }, "Metal")->setQuantity(40);

  world->createObjectAt(PrototypesCollection::getObject("FloorTile"), { -64, -32 }, "FloorTile")->setQuantity(40);
  world->createObjectAt(PrototypesCollection::getObject("FloorTile"), { -128, -32 }, "FloorTile")->setQuantity(40);

  world->createObjectAt(PrototypesCollection::getObject("Welder"), { 128, 128 }, "Welder");


  ///


  {
    auto& atmo = world->getTile({ -3, -3 })->getAtmosphere();
    atmo.d_gases.insert({ Gas::Oxygen, 200000 });
  }
  {
    auto& atmo = world->getTile({ -2, 1 })->getAtmosphere();
    atmo.d_gases.insert({ Gas::Oxygen, 100000 });
  }
  {
    auto& atmo = world->getTile({ 2, 1 })->getAtmosphere();
    atmo.d_gases.insert({ Gas::Oxygen, 50000 });
  }


  ///

  return world;
}
