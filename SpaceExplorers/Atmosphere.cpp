#include "stdafx.h"
#include "Atmosphere.h"

#include <LaggySdk/Streams.h>


bool Atmosphere::hasGases() const
{
  return !d_gases.empty();
}


int Atmosphere::getPressure() const
{
  return std::accumulate(d_gases.begin(), d_gases.end(), 0, [](int i_value, const auto& i_pair)
                         { return i_value + i_pair.second; });
}


int Atmosphere::getGasCount() const
{
  int count = 0;

  for (const auto&[_, amount] : d_gases)
  {
    if (amount > 0)
      ++count;
  }

  return count;
}

std::set<Gas> Atmosphere::getGasesTypes() const
{
  std::set<Gas> gases;

  for (const auto&[type, amount] : d_gases)
  {
    if (amount > 0)
      gases.insert(type);
  }

  return gases;
}

std::map<Gas, double> Atmosphere::getGasesRatios() const
{
  std::map<Gas, double> ratios;

  const int pressure = getPressure();
  for (const auto&[type, amount] : d_gases)
    ratios[type] = pressure == 0 ? 0 : (double)amount / pressure;

  return ratios;
}

int Atmosphere::getGasAmount(Gas i_type) const
{
  auto it = d_gases.find(i_type);
  if (it == d_gases.end())
    return 0;

  return it->second;
}


int Atmosphere::tryTakeGas(Gas i_type, int i_amount)
{
  auto it = d_gases.find(i_type);
  if (it == d_gases.end())
    return 0;

  const int takenAmount = std::min<int>(it->second, i_amount);
  it->second -= takenAmount;

  return takenAmount;
}

void Atmosphere::giveGas(Gas i_type, int i_amount)
{
  d_gases[i_type] += i_amount;
}

void Atmosphere::removeAllGases()
{
  d_gases.clear();
}


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
