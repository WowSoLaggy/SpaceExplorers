#pragma once

#include "Fwd.h"

#include <memory>


class BehaviorModelFactory
{
public:
  static std::shared_ptr<BehaviorModelBase> get(Structure& io_structure, World& io_world);
};
