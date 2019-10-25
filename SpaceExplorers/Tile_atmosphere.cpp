#include "stdafx.h"
#include "Tile.h"

#include "World.h"


void Tile::updateAtmosphere(double i_dt)
{
  // TODO: ae Refactor this whole function, separate to different ones

  static const std::vector<Sdk::Vector2I> AllTiles{ { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
  static const std::vector<Sdk::Vector2I> TwoTiles{ { 1, 0 }, { 0, 1 } };


  auto& atmo1 = getAtmosphere();
  const auto pressure1 = atmo1.getPressure();
  auto& gases1 = atmo1.getGases();


  if (!hasAtmosphere() && pressure1 == 0)
    return;


  for (const auto& offset : AllTiles)
  {
    auto* tile = d_world.getTile(d_coordsTile + offset);
    if (!tile || tile->isLeak())
    {
      atmo1.removeAllGases();
      break;
    }
  }


  if (!hasAtmosphere())
  {
    if (pressure1 > 0)
    {
      // Try to push gases to neighbor tiles

      // Collect neighbor tiles
      // There shall be no leaks or empty tiles - otherwise I would give all gases there
      // a few lines above
      std::vector<Tile*> tiles;
      for (const auto& offset : AllTiles)
      {
        auto* tile = d_world.getTile(d_coordsTile + offset);
        if (tile && tile->hasAtmosphere())
          tiles.push_back(tile);
      }

      const int tileCount = (int)tiles.size();
      int tileCountLeft = tileCount;
      for (auto* tile : tiles)
      {
        for (const auto&[type, amount] : gases1)
        {
          const auto gasToGive = amount / tileCountLeft;
          tile->getAtmosphere().giveGas(type, atmo1.tryTakeGas(type, gasToGive));
        }
        --tileCountLeft;
      }

      // If some gases still left (due to rounding error) -
      // just give it to the first tile
      if (tileCount > 0)
      {
        for (const auto&[type, amount] : gases1)
          tiles.front()->getAtmosphere().giveGas(type, amount);

        atmo1.removeAllGases();
      }
    }

    return;
  }


  for (const auto& offset : TwoTiles)
  {
    auto* tile = d_world.getTile(d_coordsTile + offset);
    if (!tile || !tile->hasAtmosphere())
      continue;

    auto& atmo2 = tile->getAtmosphere();

    const auto pressure2 = atmo2.getPressure();
    if (pressure1 == 0 && pressure2 == 0)
      continue;

    const int pressureDiff = std::abs(pressure2 - pressure1);
    if (pressureDiff <= 1)
      continue;

    const int totalGasToSpread = pressureDiff / 2;

    const double SpreadMultiplier = 5.0;

    int gasToSpread = (int)(static_cast<double>(totalGasToSpread) * i_dt * SpreadMultiplier);
    if (gasToSpread > totalGasToSpread)
      gasToSpread = totalGasToSpread;
    if (gasToSpread == 0)
      gasToSpread = 1;

    auto& srcAtmo = pressure2 > pressure1 ? atmo2: atmo1;
    auto& destAtmo = pressure2 > pressure1 ? atmo1: atmo2;

    const auto& ratios = srcAtmo.getGasesRatios();
    for (const auto&[type, amount] : srcAtmo.getGases())
    {
      const int thisGasToGive = (int)(ratios.at(type) * gasToSpread);
      destAtmo.giveGas(type, srcAtmo.tryTakeGas(type, thisGasToGive));
    }
  }
}


void Tile::setOverlayColor() const
{
  if (!d_atmosphere.hasGases())
    return;

  const std::map<Gas, Sdk::Vector4F> ColorsMap = {
    { Gas::Oxygen, { 0.0f, 0.75f, 1.0f, 1.0f} },
  { Gas::CarbonDioxide, { 1.0f, 1.0f, 0.0f, 1.0f} },
  };

  const auto ratios = d_atmosphere.getGasesRatios();

  Sdk::Vector4F color{ Sdk::Vector4F::zero() };
  for (const auto&[type, _] : d_atmosphere.getGases())
    color += ColorsMap.at(type) * (float)ratios.at(type);

  const double AveragePressure = 100000.0; // 1atm
  const int pressure = d_atmosphere.getPressure();

  const double intensity = pressure / AveragePressure * 0.5;
  color.w = (float)intensity;

  d_overlaySprite.setColor(color);
}
