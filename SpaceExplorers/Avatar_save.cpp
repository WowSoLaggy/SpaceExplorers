#include "stdafx.h"
#include "Avatar.h"

#include <LaggySdk/Streams.h>


void Avatar::writeTo(std::ostream& io_stream) const
{
  Object::writeTo(io_stream);

  Sdk::write(io_stream, d_maxSpeed);

  d_inventory.writeTo(io_stream);
}

void Avatar::readFrom(std::istream& io_stream)
{
  Object::readFrom(io_stream);

  Sdk::read(io_stream, d_maxSpeed);

  d_inventory.readFrom(io_stream, d_resourceController, d_world);
}
