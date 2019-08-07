#include "stdafx.h"
#include "Door.h"

#include <LaggySdk/Streams.h>


std::ostream& Door::write(std::ostream& io_stream) const
{
  Structure::write(io_stream);

  Sdk::write(io_stream, d_state);

  return io_stream;
}
