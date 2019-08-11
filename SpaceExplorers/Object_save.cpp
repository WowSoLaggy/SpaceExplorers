#include "stdafx.h"
#include "Object.h"

#include "Prototypes.h"

#include <LaggySdk/Streams.h>


void Object::writeTo(std::ostream& io_stream) const
{
  Sdk::writeString(io_stream, d_prototype.name);

  d_sprite.writeTo(io_stream);

  io_stream << d_position;
  
  Sdk::writeString(io_stream, d_name);
}

void Object::readFrom(std::istream& io_stream)
{
  d_sprite.readFrom(io_stream, d_resourceController);

  io_stream >> d_position;
  setPosition(d_position);

  d_name = Sdk::readString(io_stream);
}
