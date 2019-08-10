#pragma once

#include "StructurePrototype.h"

#include <string>
#include <unordered_map>


using PrototypesCollection = std::unordered_map<std::string, StructurePrototype>;

class Prototypes
{
public:
  static void load(const fs::path& i_filename);
  static const StructurePrototype& getPrototype(const std::string& i_name);

private:
  PrototypesCollection d_collection;

  static Prototypes& getInstance();
};
