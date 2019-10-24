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

  d_atmosphere.writeTo(io_stream);
  d_overlaySprite.writeTo(io_stream);
}

void Tile::readFrom(std::istream& io_stream)
{
  int layersCount;
  Sdk::read(io_stream, layersCount);

  for (int i = 0; i < layersCount; ++i)
  {
    Layer layer;
    Sdk::read(io_stream, layer);

    d_layersMap[layer] = readStructureFrom(io_stream, d_resourceController, d_world, std::move(d_coordsTile));
  }

  d_atmosphere.readFrom(io_stream);
  d_overlaySprite.readFrom(io_stream, d_resourceController);
}
