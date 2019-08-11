#include "stdafx.h"
#include "PrototypesCollection.h"

#include "SettingsProvider.h"

#include <LaggySdk/Contracts.h>
#include <LaggySdk/json.h>

#include <unordered_map>


namespace
{
  const std::unordered_map<std::string, Layer> LayerNames
  {
    { "None", Layer::None },
    { "Panneling", Layer::Panneling },
    { "Floor", Layer::Floor },
    { "Wall", Layer::Wall },
  };

  const std::unordered_map<std::string, Behavior> BehaviorNames
  {
    { "Default", Behavior::Default },
    { "Door", Behavior::Door },
  };
} // anonymous NS


PrototypesCollection& PrototypesCollection::getInstance()
{
  static PrototypesCollection collection;
  return collection;
}

void PrototypesCollection::load()
{
  auto& prototypes = getInstance();
  const auto& settings = SettingsProvider::getDefaultInternalSettings();

  prototypes.loadStructures(".//" + settings.resourcesFolder + "//" + settings.structuresPrototypesFile);
}


void PrototypesCollection::loadStructures(const fs::path& i_filename)
{
  CONTRACT_EXPECT(fs::exists(i_filename));

  std::ifstream file(i_filename.string(), std::ifstream::binary);

  Json::Reader reader;
  Json::Value root;
  CONTRACT_ASSERT(reader.parse(file, root, false));

  auto& prototypes = getInstance();
  for (const auto& protoName : root.getMemberNames())
  {
    const auto& node = root[protoName];

    StructurePrototype proto;
    proto.name = protoName;
    proto.textureFileName = node["TextureName"].asString();
    proto.layer = LayerNames.at(node["Layer"].asString());
    proto.behavior = BehaviorNames.at(node["Behavior"].asString());

    prototypes.d_collection.insert({ protoName, std::move(proto) });
  }
}


const StructurePrototype& PrototypesCollection::getPrototype(const std::string& i_name)
{
  return getInstance().d_collection.at(i_name);
}
