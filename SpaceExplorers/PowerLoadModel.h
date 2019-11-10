#pragma once

#include "BehaviorModelBase.h"


class PowerLoadModel : public BehaviorModelBase
{
public:
  PowerLoadModel(Structure& io_structure, World& io_world);

  void setRequiredPower(int i_requiredPower) { d_requiredPower = i_requiredPower; }
  int getRequiredPower() const { return d_requiredPower; }

private:
  int d_requiredPower = 0;
};
