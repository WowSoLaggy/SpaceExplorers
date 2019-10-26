#pragma once

#include "BehaviorModelBase.h"


class PowerSource : public BehaviorModelBase
{
public:
  PowerSource(Structure& io_structure, World& io_world);

  void setMaxPower(int i_maxPower);
  int getMaxPower() const { return d_maxPower; }

  void setCurPower(int i_curPower);
  int getCurPower() const { return d_curPower; }

private:
  int d_maxPower = 0;
  int d_curPower = 0;
};
