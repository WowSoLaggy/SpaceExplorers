#include "stdafx.h"
#include "Prototypes.h"


namespace Prototypes
{
  const StructurePrototype& Lattice()
  {
    static StructurePrototype prototype;
    prototype.textureFileName = "Lattice.png";
    return prototype;
  }

  const StructurePrototype& Floor()
  {
    static StructurePrototype prototype;
    prototype.textureFileName = "Floor.png";
    return prototype;
  }

  const StructurePrototype& Wall()
  {
    static StructurePrototype prototype;
    prototype.textureFileName = "Wall.png";
    return prototype;
  }
} // ns Prototypes
