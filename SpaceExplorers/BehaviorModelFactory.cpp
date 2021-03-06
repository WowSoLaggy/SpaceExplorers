#include "stdafx.h"
#include "BehaviorModelFactory.h"

#include "Behaviors.h"
#include "ContainerModel.h"
#include "DoorModel.h"
#include "PowerLoadModel.h"
#include "PowerSourceModel.h"
#include "Structure.h"

#include <LaggySdk/Contracts.h>


std::shared_ptr<BehaviorModelBase> BehaviorModelFactory::get(Structure& io_structure, World& io_world)
{
  const auto behavior = io_structure.getPrototype().behavior;

  if (behavior == Behavior::Default)
    return std::make_shared<BehaviorModelBase>(io_structure, io_world);
  else if (behavior == Behavior::Door)
    return std::make_shared<DoorModel>(io_structure, io_world);
  else if (behavior == Behavior::Container)
    return std::make_shared<ContainerModel>(io_structure, io_world);
  else if (behavior == Behavior::PowerSource)
    return std::make_shared<PowerSourceModel>(io_structure, io_world);
  else if (behavior == Behavior::PowerLoad)
    return std::make_shared<PowerLoadModel>(io_structure, io_world);

  CONTRACT_EXPECT(false);
}
