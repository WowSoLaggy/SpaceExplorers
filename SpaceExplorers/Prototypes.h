#pragma once

#include "Fwd.h"
#include "Traits.h"

#include <string>


struct StructurePrototype
{
  std::string name = "";

  std::string textureFileName = "";
  Behavior behavior = Behavior::Default;
  Layer layer = Layer::None;
  bool isPassable = true;
};


struct Receipt
{
  const StructurePrototype* input = nullptr;
  const StructurePrototype* output = nullptr;
  const double time = 0;
  const std::string resultName;
  const ObjectPrototype* result = nullptr;
};

struct ObjectPrototype
{
  std::string name = "";

  std::string textureFileName = "";
  bool isStackable = false;

  bool hasReceipts() const { return !receipts.empty(); }
  std::vector<Receipt> receipts;
  bool consumedOnBuild = false;
};
