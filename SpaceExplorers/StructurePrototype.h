#pragma once

#include "Traits.h"

#include <string>


struct StructurePrototype
{
  std::string name = "";

  std::string textureFileName = "";
  Behavior behavior = Behavior::None;
  Layer layer = Layer::None;
};
