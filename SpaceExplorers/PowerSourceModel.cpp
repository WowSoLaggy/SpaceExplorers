#include "stdafx.h"
#include "PowerSourceModel.h"

#include "Structure.h"


PowerSourceModel::PowerSourceModel(Structure& io_structure, World& io_world)
  : BehaviorModelBase(io_structure, io_world)
{
  setMaxPower(io_structure.getPrototype().maxPower);
}


void PowerSourceModel::setMaxPower(int i_maxPower)
{
  d_maxPower = i_maxPower;
  d_curPower = std::min(d_maxPower, d_curPower);
}

void PowerSourceModel::setCurPower(int i_curPower)
{
  d_curPower = std::min(d_maxPower, i_curPower);
}
