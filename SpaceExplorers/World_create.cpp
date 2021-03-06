#include "stdafx.h"
#include "World.h"

#include "ContainerModel.h"
#include "DoorModel.h"
#include "PowerSourceModel.h"
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

  auto createObject = [&](const std::string& i_protoName, int i_quantity = 1) -> ObjectPtr {
    const auto& proto = PrototypesCollection::getObject(i_protoName);
    auto objectPtr = std::make_shared<Object>(i_resourceController, *world, proto);
    objectPtr->setName(i_protoName);
    objectPtr->setQuantity(i_quantity);
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
  create(0, -2, "Door")->getBehaviorModel<DoorModel>()->setState(DoorModel::State::Open);
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


  create(3, -2, "Lamp")->setOrientation(Sdk::Side::Left);
  create(-2, -4, "Lamp")->setOrientation(Sdk::Side::Down);
  create(-2, 3, "Lamp")->setOrientation(Sdk::Side::Up);
  create(-5, 1, "Lamp")->setOrientation(Sdk::Side::Up);
  create(3, 1, "Lamp")->setOrientation(Sdk::Side::Left);


  ///


  {
    auto crate = create(-6, -1, "Crate");
    auto& container = crate->getBehaviorModel<ContainerModel>()->getContainer();

    auto rods = createObject("Rods");
    rods->setQuantity(200);
    container.setItem(0, rods);

    auto metal = createObject("Metal");
    metal->setQuantity(200);
    container.setItem(1, metal);

    auto floorTiles = createObject("FloorTile");
    floorTiles->setQuantity(200);
    container.setItem(2, floorTiles);
  }


  ///


  {
    auto tom = world->createAvatarAt(PrototypesCollection::getObject("Man"), { -96, -32 }, "Tom");
    tom->getInventory().setSize(8);
    tom->getInventory().setItem(0, createObject("Crowbar"));
    tom->getInventory().setItem(1, createObject("Welder"));
    tom->getInventory().setItem(2, createObject("CuttingTorch"));
    tom->getInventory().setItem(3, createObject("Screwdriver"));

    tom->getInventory().setItem(5, createObject("Lamp_packed", 10));
    tom->getInventory().setItem(6, createObject("Wires_packed", 40));
  }

  
  ///


  {
    auto& atmo = world->getTile({ -3, -3 })->getAtmosphere();
    atmo.giveGas(Gas::Oxygen, 200000);
  }
  {
    auto& atmo = world->getTile({ -2, 1 })->getAtmosphere();
    atmo.giveGas(Gas::Oxygen, 100000);
  }
  {
    auto& atmo = world->getTile({ 2, 1 })->getAtmosphere();
    atmo.giveGas(Gas::Nitrogen, 750000);
  }
  {
    auto& atmo = world->getTile({ -5, -1 })->getAtmosphere();
    atmo.giveGas(Gas::CarbonDioxide, 750000);
  }


  ///


  auto capacitor = create(1, -3, "Capacitor");
  capacitor->getBehaviorModel<PowerSourceModel>()->setCurPower(500);


  ///

  return world;
}
