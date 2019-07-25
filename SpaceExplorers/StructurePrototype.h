#pragma once

#include "Behaviors.h"

#include <string>


struct StructurePrototype
{
  std::string textureFileName = "";
  Behavior behavior = Behavior::None;
};
