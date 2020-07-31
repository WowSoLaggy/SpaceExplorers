#pragma once

#include "BuildContext.h"
#include "Fwd.h"


class BuildManager
{
public:
  /// Returns nullptr if cannot be built. Returns BuildContext otherwise
  static BuildContextPtr tryBuild(
    StructurePtr i_structure,
    ObjectPtr i_tool,
    const World& i_world,
    const Sdk::Vector2I& i_where);
};
