#pragma once

#include "Fwd.h"
#include "Prototypes.h"

#include <LaggySdk/Vector.h>

#include <memory>


struct BuildContext
{
  BuildContext(StructurePtr i_object, ObjectPtr i_tool,
               Receipt i_receipt, Sdk::Vector2I i_coords,
               AttachmentPosition i_attachPosition)
    : object(i_object)
    , tool(i_tool)
    , receipt(i_receipt)
    , coords(i_coords)
    , attachPosition(i_attachPosition)
  {
  }

  bool takesTime() const
  {
    return receipt.time > 0.01;
  }

  StructurePtr object;
  ObjectPtr tool;
  Receipt receipt;
  Sdk::Vector2I coords;
  AttachmentPosition attachPosition = AttachmentPosition::Center;
};
