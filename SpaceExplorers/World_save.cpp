#include "stdafx.h"
#include "World.h"

#include "ObjectsReader.h"

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

  Sdk::write(io_stream, (int)d_objects.size());
  for (const auto& object : d_objects)
    object->writeTo(io_stream);

  Sdk::write(io_stream, (int)d_avatars.size());
  for (const auto&[_, avatar] : d_avatars)
    avatar->writeTo(io_stream);
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

  int objectsCount;
  Sdk::read(io_stream, objectsCount);
  for (int i = 0; i < objectsCount; ++i)
  {
    auto objectPtr = readObjectFrom(io_stream, i_resourceController, *world);
    world->d_objects.push_back(std::move(objectPtr));
  }

  int avatarsCount;
  Sdk::read(io_stream, avatarsCount);
  for (int i = 0; i < avatarsCount; ++i)
  {
    auto avatarPtr = readAvatarFrom(io_stream, i_resourceController, *world);
    world->d_avatars.insert({ avatarPtr->getName(), std::move(avatarPtr) });
  }

  return world;
}
