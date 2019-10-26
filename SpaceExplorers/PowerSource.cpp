#include "stdafx.h"
#include "PowerSource.h"


PowerSource::PowerSource(Structure& io_structure, World& io_world)
  : BehaviorModelBase(io_structure, io_world)
{
}


void PowerSource::setMaxPower(int i_maxPower)
{
  d_maxPower = i_maxPower;
  d_curPower = std::min(d_maxPower, d_curPower);
}

void PowerSource::setCurPower(int i_curPower)
{
  d_curPower = std::min(d_maxPower, i_curPower);
}
