#include "stdafx.h"
#include "World.h"

#include <LaggyDx/Sprite.h>
#include <LaggySdk/Streams.h>


void World::writeTo(std::ostream& io_stream) const
{
  d_background.writeTo(io_stream);

  Sdk::write(io_stream, (int)d_tilesMap.size());
  for (const auto&[coords, tile] : d_tilesMap)
  {
    io_stream << coords;
    tile.writeTo(io_stream);
  }
}

std::unique_ptr<World> World::readFrom(std::istream& io_stream,
                                       Sdk::Vector2I i_backgroundSize,
                                       Dx::IResourceController& i_resourceController)
{
  auto world = createEmptyWorld(i_backgroundSize, i_resourceController);

  world->d_background.readFrom(io_stream, i_resourceController);

  int tilesCount;
  Sdk::read(io_stream, tilesCount);
  for (int i = 0; i < tilesCount; ++i)
  {
    Sdk::Vector2I coords;
    io_stream >> coords;

    world->d_tilesMap[coords].readFrom(io_stream, i_resourceController, coords);
  }

  return world;
}
