#include "stdafx.h"
#include "PowerLoadModel.h"

#include "Structure.h"


PowerLoadModel::PowerLoadModel(Structure& io_structure, World& io_world)
  : BehaviorModelBase(io_structure, io_world)
{
  setRequiredPower(io_structure.getPrototype().requiredPower);
}
