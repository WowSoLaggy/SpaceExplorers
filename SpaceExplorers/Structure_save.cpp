#include "stdafx.h"
#include "Structure.h"

#include <LaggySdk/Streams.h>


std::ostream& Structure::write(std::ostream& io_stream) const
{
  Sdk::write(io_stream, (int)d_prototype.name.length());
  io_stream << d_prototype.name;

  bool hasSprite = d_sprite != nullptr;
  io_stream << hasSprite;
  if (hasSprite)
    io_stream << *d_sprite;

  io_stream << d_coordsTile;

  return io_stream;
}

std::ostream& operator<<(std::ostream& io_stream, const Structure& i_structure)
{
  return i_structure.write(io_stream);
}
