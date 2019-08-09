#include "stdafx.h"
#include "StructureReader.h"

#include "Door.h"
#include "Prototypes.h"


StructurePtr readStructureFrom(std::istream& io_stream,
                               Dx::IResourceController& i_resourceController,
                               Sdk::Vector2I i_coordsTile)
{
  const auto prototypeName = Sdk::readString(io_stream);
  const auto& prototype = Prototypes::getPrototype(prototypeName);

  StructurePtr structurePtr;
  if (prototype.behavior == Behavior::Door)
    structurePtr = std::make_shared<Door>(i_resourceController, prototype, std::move(i_coordsTile));
  else
    structurePtr = std::make_shared<Structure>(i_resourceController, prototype, std::move(i_coordsTile));

  structurePtr->readFrom(io_stream);

  return structurePtr;
}
