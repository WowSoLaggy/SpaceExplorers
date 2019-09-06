#include "stdafx.h"
#include "BehaviorModelFactory.h"

#include "Behaviors.h"
#include "DoorModel.h"
#include "Structure.h"


std::shared_ptr<BehaviorModelBase> BehaviorModelFactory::get(Structure& io_structure)
{
  const auto behavior = io_structure.getPrototype().behavior;

  if (behavior == Behavior::Door)
    return std::make_shared<DoorModel>(io_structure);

  return std::make_shared<BehaviorModelBase>(io_structure);
}