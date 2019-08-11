#pragma once

#include "Prototypes.h"

#include <string>
#include <unordered_map>


class PrototypesCollection
{
public:
  static void load(const fs::path& i_filename);
  static const StructurePrototype& getPrototype(const std::string& i_name);

private:
  std::unordered_map<std::string, StructurePrototype> d_collection;

  static PrototypesCollection& getInstance();
};
