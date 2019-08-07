#include "stdafx.h"
#include "World.h"

#include <LaggySdk/Streams.h>


std::ostream& operator<<(std::ostream& io_stream, const World& i_world)
{
  io_stream << i_world.d_background;

  Sdk::write(io_stream, (int)i_world.d_tilesMap.size());
  for (const auto&[coords, tile] : i_world.d_tilesMap)
  {
    io_stream << coords;
    io_stream << tile;
  }

  return io_stream;
}
