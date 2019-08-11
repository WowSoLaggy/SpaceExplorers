#include "stdafx.h"
#include "Object.h"

#include <LaggySdk/Streams.h>


void Object::writeTo(std::ostream& io_stream) const
{
  Sdk::writeString(io_stream, d_prototype.name);

  d_sprite.writeTo(io_stream);

  io_stream << d_position;
}

void Object::readFrom(std::istream& io_stream)
{
  d_sprite.readFrom(io_stream, d_resourceController);

  io_stream >> d_position;
}
