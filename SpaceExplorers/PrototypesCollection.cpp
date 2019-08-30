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
  prototypes.loadObjects(".//" + settings.resourcesFolder + "//" + settings.objectsPrototypesFile);
}


void PrototypesCollection::loadStructures(const fs::path& i_filename)
{
  CONTRACT_EXPECT(fs::exists(i_filename));

  std::ifstream file(i_filename.string(), std::ifstream::binary);

  Json::Reader reader;
  Json::Value root;
  CONTRACT_ASSERT(reader.parse(file, root, false));

  for (const auto& protoName : root.getMemberNames())
  {
    const auto& node = root[protoName];

    StructurePrototype proto;
    proto.name = protoName;
    proto.textureFileName = node["TextureName"].asString();
    proto.layer = LayerNames.at(node["Layer"].asString());
    proto.behavior = BehaviorNames.at(node["Behavior"].asString());
    proto.isPassable = node["Passable"].asBool();

    d_collectionStructures.insert({ protoName, std::move(proto) });
  }
}

void PrototypesCollection::loadObjects(const fs::path& i_filename)
{
  CONTRACT_EXPECT(fs::exists(i_filename));

  std::ifstream file(i_filename.string(), std::ifstream::binary);

  Json::Reader reader;
  Json::Value root;
  CONTRACT_ASSERT(reader.parse(file, root, false));

  for (const auto& protoName : root.getMemberNames())
  {
    const auto& node = root[protoName];

    ObjectPrototype proto;

    proto.name = protoName;
    proto.textureFileName = node["TextureName"].asString();
    proto.isStackable = node["Stackable"].asBool();

    const std::string ReceiptsNodeName = "Receipts";
    if (node.find(ReceiptsNodeName.data(), ReceiptsNodeName.data() + ReceiptsNodeName.length()))
    {
      proto.consumedOnBuild = node["ConsumedOnBuild"].asBool();

      const auto& receiptsNode = node[ReceiptsNodeName];
      CONTRACT_ASSERT(receiptsNode.isArray());

      for (const auto& receiptNode : receiptsNode)
      {
        const std::string inputName = receiptNode["Input"].asString();
        const std::string outputName = receiptNode["Output"].asString();

        const StructurePrototype* inputProto = nullptr;
        const StructurePrototype* outputProto = nullptr;

        if (!inputName.empty())
          inputProto = &getStructure(inputName);
        if (!outputName.empty())
          outputProto = &getStructure(outputName);

        double time = 0;
        const std::string TimeNodeName = "Time";
        if (receiptNode.find(TimeNodeName.data(), TimeNodeName.data() + TimeNodeName.length()))
          time = receiptNode[TimeNodeName].asDouble();

        std::string resultProtoName;
        const std::string ResultNodeName = "Result";
        if (receiptNode.find(ResultNodeName.data(), ResultNodeName.data() + ResultNodeName.length()))
          resultProtoName = receiptNode[ResultNodeName].asString();

        proto.receipts.emplace_back<Receipt>({ inputProto, outputProto, time, resultProtoName });
      }
    }

    d_collectionObjects.insert({ protoName, std::move(proto) });
  }

  // Now find all prototypes

  for (auto& [_, proto] : d_collectionObjects)
  {
    for (auto& receipt : proto.receipts)
    {
      if (!receipt.resultName.empty())
        receipt.result = &getObject(receipt.resultName);
    }
  }
}


const StructurePrototype* PrototypesCollection::findStructure(const std::string& i_name)
{
  auto it = getInstance().d_collectionStructures.find(i_name);
  return it != getInstance().d_collectionStructures.end() ? &it->second : nullptr;
}

const StructurePrototype& PrototypesCollection::getStructure(const std::string& i_name)
{
  return getInstance().d_collectionStructures.at(i_name);
}


const ObjectPrototype* PrototypesCollection::findObject(const std::string& i_name)
{
  auto it = getInstance().d_collectionObjects.find(i_name);
  return it != getInstance().d_collectionObjects.end() ? &it->second : nullptr;
}

const ObjectPrototype& PrototypesCollection::getObject(const std::string& i_name)
{
  return getInstance().d_collectionObjects.at(i_name);
}
