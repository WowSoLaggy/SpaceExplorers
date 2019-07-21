#include "stdafx.h"
#include "Prototypes.h"


namespace Prototypes
{
  const StructurePrototype& getLattice()
  {
    static StructurePrototype prototype;
    prototype.textureFileName = "Lattice_64_64_1_a.png";
    return prototype;
  }
} // ns Prototypes
