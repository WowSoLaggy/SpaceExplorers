#include "stdafx.h"
#include "Door.h"

#include <LaggyDx/AnimatedSprite.h>
#include <LaggySdk/Streams.h>


void Door::writeTo(std::ostream& io_stream) const
{
  // TODO: ae Remove this duplication

  Sdk::writeString(io_stream, d_prototype.name);

  bool hasSprite = d_sprite != nullptr;
  Sdk::write(io_stream, hasSprite);
  if (hasSprite)
    d_sprite->writeTo(io_stream);

  io_stream << d_coordsTile;

  // Specific

  Sdk::write(io_stream, d_state);
}

void Door::readFrom(std::istream& io_stream)
{
  // TODO: ae Remove this duplication

  bool hasSprite;
  Sdk::read(io_stream, hasSprite);
  if (hasSprite)
  {
    d_sprite = std::make_shared<Dx::AnimatedSprite>();
    d_sprite->readFrom(io_stream, d_resourceController);
  }

  io_stream >> d_coordsTile;

  // Specific

  Sdk::read(io_stream, d_state);
}
