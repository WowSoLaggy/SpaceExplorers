#include "stdafx.h"
#include "Prototypes.h"


namespace Prototypes
{
  const StructurePrototype& Lattice()
  {
    static StructurePrototype prototype;
    prototype.textureFileName = "Lattice.png";
    prototype.layer = Layer::Panneling;
    return prototype;
  }

  const StructurePrototype& Floor()
  {
    static StructurePrototype prototype;
    prototype.textureFileName = "Floor.png";
    prototype.layer = Layer::Floor;
    return prototype;
  }

  const StructurePrototype& Wall()
  {
    static StructurePrototype prototype;
    prototype.textureFileName = "Wall.png";
    prototype.layer = Layer::Wall;
    return prototype;
  }

  const StructurePrototype& Door()
  {
    static StructurePrototype prototype;
    prototype.textureFileName = "Door.png";
    prototype.behavior = Behavior::Door;
    prototype.layer = Layer::Wall;
    return prototype;
  }
} // ns Prototypes
