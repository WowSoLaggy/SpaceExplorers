#pragma once

#include "Prototypes.h"

#include <string>
#include <unordered_map>


class PrototypesCollection
{
public:
  static void load();

  static const StructurePrototype* findStructure(const std::string& i_name);
  static const StructurePrototype& getStructure(const std::string& i_name);
  
  static const ObjectPrototype* findObject(const std::string& i_name);
  static const ObjectPrototype& getObject(const std::string& i_name);

private:
  std::unordered_map<std::string, StructurePrototype> d_collectionStructures;
  std::unordered_map<std::string, ObjectPrototype> d_collectionObjects;

  void loadStructures(const fs::path& i_filename);
  void loadObjects(const fs::path& i_filename);

private:
  static PrototypesCollection& getInstance();
};
