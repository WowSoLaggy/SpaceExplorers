#include "stdafx.h"
#include "World.h"

#include "Prototypes.h"

#include <LaggyDx/IResourceController.h>


std::unique_ptr<World> World::createTestWorld(const Dx::IResourceController& i_resourceController)
{
  auto world = std::make_unique<World>();

  Structure lattice(i_resourceController, Prototypes::getLattice(), { 0, 0 });
  world->d_structures.push_back(std::move(lattice));

  return world;
}
