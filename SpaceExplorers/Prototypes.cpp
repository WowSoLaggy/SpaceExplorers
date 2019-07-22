#include "stdafx.h"
#include "Prototypes.h"


namespace Prototypes
{
  const StructurePrototype& Lattice()
  {
    static StructurePrototype prototype;
    prototype.textureFileName = "Lattice_64_64_1_a.png";
    return prototype;
  }

  const StructurePrototype& Floor()
  {
    static StructurePrototype prototype;
    prototype.textureFileName = "Floor_64_64_1_a.png";
    return prototype;
  }

  const StructurePrototype& Wall()
  {
    static StructurePrototype prototype;
    prototype.textureFileName = "Wall_64_64_1_a.png";
    return prototype;
  }
} // ns Prototypes
