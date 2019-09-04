#include "stdafx.h"
#include "Structure.h"

#include <LaggySdk/Streams.h>


void Structure::writeTo(std::ostream& io_stream) const
{
  Sdk::writeString(io_stream, d_prototype.name);

  d_sprite.writeTo(io_stream);

  io_stream << d_coordsTile;
}

void Structure::readFrom(std::istream& io_stream)
{
  d_sprite.readFrom(io_stream, d_resourceController);

  io_stream >> d_coordsTile;
}
