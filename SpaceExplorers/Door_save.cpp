#include "stdafx.h"
#include "Door.h"

#include <LaggyDx/AnimatedSprite.h>
#include <LaggySdk/Streams.h>


void Door::writeTo(std::ostream& io_stream) const
{
  Structure::writeTo(io_stream);

  Sdk::write(io_stream, d_state);
}

void Door::readFrom(std::istream& io_stream)
{
  Structure::readFrom(io_stream);

  Sdk::read(io_stream, d_state);
}
