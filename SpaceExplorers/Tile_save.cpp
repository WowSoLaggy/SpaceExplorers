#include "stdafx.h"
#include "Tile.h"

#include <LaggySdk/Contracts.h>
#include <LaggySdk/Streams.h>


std::ostream& operator<<(std::ostream& io_stream, const Tile& i_tile)
{
  Sdk::write(io_stream, (int)i_tile.d_layersMap.size());
  for (const auto&[layer, structurePtr] : i_tile.d_layersMap)
  {
    CONTRACT_EXPECT(structurePtr);

    Sdk::write(io_stream, layer);
    io_stream << *structurePtr;
  }

  return io_stream;
}
