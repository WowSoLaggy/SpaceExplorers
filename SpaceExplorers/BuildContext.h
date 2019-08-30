#pragma once

#include "Fwd.h"
#include "Prototypes.h"

#include <LaggySdk/Vector.h>

#include <memory>


struct BuildContext
{
  BuildContext(StructurePtr i_object, ObjectPtr i_tool,
               Receipt i_receipt, Sdk::Vector2I i_tileCoords)
    : d_object(i_object)
    , d_tool(i_tool)
    , d_receipt(i_receipt)
    , d_tileCoords(i_tileCoords)
  {
  }

  StructurePtr d_object;
  ObjectPtr d_tool;
  Receipt d_receipt;
  Sdk::Vector2I d_tileCoords;
};
