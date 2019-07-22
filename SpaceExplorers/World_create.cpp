#include "stdafx.h"
#include "World.h"

#include "Prototypes.h"
#include "Structure.h"

#include <LaggyDx/IResourceController.h>


std::unique_ptr<World> World::createTestWorld(const Dx::IResourceController& i_resourceController)
{
  auto world = std::make_unique<World>();

  StructurePtr lattice = std::make_shared<Structure>(
    i_resourceController, Prototypes::getLattice(), Sdk::Vector2{ 0, 0 });
  world->d_structures.push_back(std::move(lattice));

  return world;
}
