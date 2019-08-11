#pragma once

#include "Traits.h"

#include <string>


struct StructurePrototype
{
  std::string name = "";

  std::string textureFileName = "";
  Behavior behavior = Behavior::Default;
  Layer layer = Layer::None;
};

struct ObjectPrototype
{
  std::string name = "";

  std::string textureFileName = "";
};

