#include "stdafx.h"
#include "Tile.h"

#include "ObjectsReader.h"

#include <LaggySdk/Contracts.h>
#include <LaggySdk/Streams.h>


void Tile::writeTo(std::ostream& io_stream) const
{
  Sdk::write(io_stream, (int)d_layersMap.size());
  for (const auto&[layer, structurePtr] : d_layersMap)
  {
    CONTRACT_EXPECT(structurePtr);

    Sdk::write(io_stream, layer);
    structurePtr->writeTo(io_stream);
  }
}

void Tile::readFrom(std::istream& io_stream,
                    Dx::IResourceController& i_resourceController,
                    const Sdk::Vector2I& i_coords)
{
  int layersCount;
  Sdk::read(io_stream, layersCount);

  for (int i = 0; i < layersCount; ++i)
  {
    Layer layer;
    Sdk::read(io_stream, layer);

    d_layersMap[layer] = readStructureFrom(io_stream, i_resourceController, std::move(i_coords));
  }
}
