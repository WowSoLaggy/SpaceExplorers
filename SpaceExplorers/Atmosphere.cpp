#include "stdafx.h"
#include "Atmosphere.h"

#include <LaggySdk/Streams.h>


void Atmosphere::writeTo(std::ostream& io_stream) const
{
  const int countGases = (int)d_gases.size();
  Sdk::write(io_stream, countGases);

  for (const auto&[type, amount] : d_gases)
  {
    Sdk::write(io_stream, type);
    Sdk::write(io_stream, amount);
  }
}

void Atmosphere::readFrom(std::istream& io_stream)
{
  int countGases;
  Sdk::read(io_stream, countGases);

  for (int i = 0; i < countGases; ++i)
  {
    Gas type;
    Sdk::read(io_stream, type);

    int amount;
    Sdk::read(io_stream, amount);

    d_gases.insert({ type, amount });
  }
}
