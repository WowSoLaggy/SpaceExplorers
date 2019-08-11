#pragma once

#include "Prototypes.h"

#include <string>
#include <unordered_map>


class PrototypesCollection
{
public:
  static void load();

  static const StructurePrototype& getStructure(const std::string& i_name);
private:
  std::unordered_map<std::string, StructurePrototype> d_collectionStructures;
  void loadStructures(const fs::path& i_filename);

private:
  static PrototypesCollection& getInstance();
};
