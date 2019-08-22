#include "stdafx.h"
#include "Container.h"

#include "Object.h"
#include "ObjectsReader.h"

#include <LaggySdk/Streams.h>


void Container::writeTo(std::ostream& io_stream) const
{
  Sdk::write(io_stream, d_size);

  for (const auto& item : d_items)
  {
    bool hasItem = item != nullptr;
    Sdk::write(io_stream, hasItem);
    if (hasItem)
      item->writeTo(io_stream);
  }
}

void Container::readFrom(std::istream& io_stream, Dx::IResourceController& i_resouceContoller, const World& i_world)
{
  int size;
  Sdk::read(io_stream, size);
  setSize(size);

  for (int i = 0; i < size; ++i)
  {
    bool hasItem;
    Sdk::read(io_stream, hasItem);
    if (hasItem)
    {
      auto object = readObjectFrom(io_stream, i_resouceContoller, i_world);
      setItem(i, object);
    }
  }
}
