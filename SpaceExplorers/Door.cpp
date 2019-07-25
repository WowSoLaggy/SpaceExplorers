#include "stdafx.h"
#include "Door.h"

#include <LaggySdk/Contracts.h>


Door::Door(
  const Dx::IResourceController& i_resourceController,
  const StructurePrototype& i_prototype,
  Sdk::Vector2I i_coordsTile)
  : Structure(i_resourceController, i_prototype, i_coordsTile)
{
}


Actions Door::getDefaultAction() const
{
  return Actions::Open;
}

void Door::interact(Actions i_action)
{
  if (i_action == Actions::None)
    return;

  if (i_action == Actions::Default)
  {
    auto defaultAction = getDefaultAction();
    CONTRACT_EXPECT(defaultAction != Actions::Default);
    interact(defaultAction);
  }
}
