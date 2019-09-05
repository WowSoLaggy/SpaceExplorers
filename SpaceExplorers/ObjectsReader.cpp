#include "stdafx.h"
#include "ObjectsReader.h"

#include "PrototypesCollection.h"


StructurePtr readStructureFrom(std::istream& io_stream,
                               Dx::IResourceController& i_resourceController,
                               Sdk::Vector2I i_coordsTile)
{
  const auto prototypeName = Sdk::readString(io_stream);
  const auto& prototype = PrototypesCollection::getStructure(prototypeName);

  StructurePtr structurePtr = std::make_shared<Structure>(i_resourceController, prototype, std::move(i_coordsTile));

  structurePtr->readFrom(io_stream);

  return structurePtr;
}


ObjectPtr readObjectFrom(std::istream& io_stream,
                         Dx::IResourceController& i_resourceController,
                         World& i_world)
{
  const auto prototypeName = Sdk::readString(io_stream);
  const auto& prototype = PrototypesCollection::getObject(prototypeName);

  auto objectPtr = std::make_shared<Object>(i_resourceController, i_world, prototype);
  objectPtr->readFrom(io_stream);

  return objectPtr;
}

AvatarPtr readAvatarFrom(std::istream& io_stream,
                         Dx::IResourceController& i_resourceController,
                         World& i_world)
{
  const auto prototypeName = Sdk::readString(io_stream);
  const auto& prototype = PrototypesCollection::getObject(prototypeName);

  auto avatarPtr = std::make_shared<Avatar>(i_resourceController, i_world, prototype);
  avatarPtr->readFrom(io_stream);

  return avatarPtr;
}

