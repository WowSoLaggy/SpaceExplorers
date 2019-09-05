#pragma once

#include "BehaviorModelBase.h"
#include "Fwd.h"

#include <memory>


class BehaviorModelFactory
{
public:
  static std::unique_ptr<BehaviorModelBase> get(Structure& io_structure);
};
