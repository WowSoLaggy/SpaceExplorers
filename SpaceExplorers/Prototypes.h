#pragma once

#include "StructurePrototype.h"


namespace Prototypes
{
  const StructurePrototype& getPrototype(const std::string& i_name);

  const StructurePrototype& Lattice();
  const StructurePrototype& Floor();
  const StructurePrototype& Wall();
  const StructurePrototype& Door();

} // ns Prototypes
