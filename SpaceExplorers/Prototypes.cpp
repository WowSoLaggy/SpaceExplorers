#include "stdafx.h"
#include "Prototypes.h"


namespace Prototypes
{
  const StructurePrototype& getPrototype(const std::string& i_name)
  {
    static const std::map<std::string, std::function<const StructurePrototype&()>> GettersMap
    {
      { "Lattice", Lattice },
      { "Floor", Floor },
      { "Wall", Wall },
      { "Door", Door },
    };

    return GettersMap.at(i_name)();
  }


  const StructurePrototype& Lattice()
  {
    static StructurePrototype prototype;
    prototype.name = "Lattice";
    prototype.textureFileName = "Lattice.png";
    prototype.layer = Layer::Panneling;
    return prototype;
  }

  const StructurePrototype& Floor()
  {
    static StructurePrototype prototype;
    prototype.name = "Floor";
    prototype.textureFileName = "Floor.png";
    prototype.layer = Layer::Floor;
    return prototype;
  }

  const StructurePrototype& Wall()
  {
    static StructurePrototype prototype;
    prototype.name = "Wall";
    prototype.textureFileName = "Wall.png";
    prototype.layer = Layer::Wall;
    return prototype;
  }

  const StructurePrototype& Door()
  {
    static StructurePrototype prototype;
    prototype.name = "Door";
    prototype.textureFileName = "Door.png";
    prototype.behavior = Behavior::Door;
    prototype.layer = Layer::Wall;
    return prototype;
  }
} // ns Prototypes
