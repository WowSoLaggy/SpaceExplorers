#include "stdafx.h"
#include "BuildManager.h"

#include "Object.h"
#include "Structure.h"
#include "Utils.h"
#include "World.h"


BuildContextPtr BuildManager::tryBuild(
  StructurePtr i_structure,
  ObjectPtr i_tool,
  const World& i_world,
  const Sdk::Vector2I& i_where)
{
  if (!i_tool)
    return {};
  if (i_structure && !i_structure->isStructure())
    return {};

  const StructurePrototype* inputProto = i_structure ? &i_structure->getPrototype() : nullptr;
  const auto& receipts = i_tool->getPrototype().receipts;
  auto it = std::find_if(receipts.cbegin(), receipts.cend(), [&](const auto& i_receipt) {
    return i_receipt.input == inputProto;
  });
  if (it == receipts.cend())
    return {};

  const auto& receipt = *it;
  const auto* outputProto = receipt.output;
  const auto tileCoords = worldToTile(i_where);

  const bool buildingAttachment = outputProto && outputProto->isAttachment;
  if (!buildingAttachment)
  {
    // No output proto or not an attachment

    auto* tile = i_world.getTile(tileCoords);
    if (tile)
    {
      const bool sameLayer = inputProto && outputProto && inputProto->layer == outputProto->layer;
      const bool outputLayerOccupied = outputProto && tile->hasStructureOnLayer(outputProto->layer);
      if (!sameLayer && outputLayerOccupied)
      {
        // Output layer shall not be occupied
        return {};
      }
    }
  }
  else
  {
    // Building attachment

    if (!i_structure)
    {
      // Nothing to attach to
      return {};
    }

    const auto canBeAttachedTo = outputProto->attachedTo;
    if (!i_structure->canHaveAttachment(canBeAttachedTo))
      return {};
  }

  if (!i_world.checkSupport(tileCoords))
    return {};

  // Impassable structures cannot be built with collision
  if (outputProto && !outputProto->isPassable && i_world.checkIntersectWithAnyObject(getTileRect(tileCoords)))
    return {};


  return std::make_shared<BuildContext>(i_structure, i_tool, receipt, i_where);
}
