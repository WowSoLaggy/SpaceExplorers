#include "stdafx.h"
#include "Tile.h"

#include "World.h"

#include <LaggySdk/Contracts.h>


namespace
{
  const std::vector<Sdk::Vector2I> AllTiles{ { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
  const std::vector<Sdk::Vector2I> TwoTiles{ { 1, 0 }, { 0, 1 } };

  bool hasLeak(World& i_world, const Sdk::Vector2I& i_coordsTile)
  {
    for (const auto& offset : AllTiles)
    {
      auto* tile = i_world.getTile(i_coordsTile + offset);
      if (!tile || tile->isLeak())
        return true;
    }

    return false;
  }

  void shareGases(Atmosphere& i_srcAtmo, Atmosphere& i_destAtmo, int i_amount)
  {
    const auto& ratios = i_srcAtmo.getGasesRatios();
    for (const auto&[type, amount] : i_srcAtmo.getGases())
    {
      const int thisGasToGive = (int)(ratios.at(type) * i_amount);
      i_destAtmo.giveGas(type, i_srcAtmo.tryTakeGas(type, thisGasToGive));
    }
  }

  void tryPushGasesToNeighbors(Atmosphere& i_atmo, World& i_world, const Sdk::Vector2I& i_coordsTile)
  {
    // Try to push gases to neighbor tiles

    // Collect neighbor tiles
    // There shall be no leaks or empty tiles
    std::vector<Tile*> tiles;
    for (const auto& offset : AllTiles)
    {
      auto* tile = i_world.getTile(i_coordsTile + offset);
      if (tile && tile->hasAtmosphere())
        tiles.push_back(tile);
    }

    const int tileCount = (int)tiles.size();
    int tileCountLeft = tileCount;
    for (auto* tile : tiles)
    {
      for (const auto&[type, amount] : i_atmo.getGases())
      {
        const auto gasToGive = amount / tileCountLeft;
        tile->getAtmosphere().giveGas(type, i_atmo.tryTakeGas(type, gasToGive));
      }
      --tileCountLeft;
    }

    // If some gases still left (due to rounding error) -
    // just give it to the first tile
    if (tileCount > 0)
    {
      for (const auto&[type, amount] : i_atmo.getGases())
        tiles.front()->getAtmosphere().giveGas(type, amount);

      i_atmo.removeAllGases();
    }
  }

  void diffuseGases(Atmosphere& i_atmo1, Atmosphere& i_atmo2, double i_dt)
  {
    if (i_atmo1.getGasCount() == 0 || i_atmo2.getGasCount() == 0)
      return;

    std::set<Gas> gasesList = i_atmo1.getGasesTypes();
    const std::set<Gas> gasesList2 = i_atmo2.getGasesTypes();
    gasesList.insert(gasesList2.cbegin(), gasesList2.cend());

    if (gasesList.size() < 2)
      return;

    std::map<int, Gas> diffs;
    for (const auto& type : gasesList)
      diffs.insert({ i_atmo2.getGasAmount(type) - i_atmo1.getGasAmount(type), type});
    
    if (diffs.size() < 2)
    {
      // It can still have only 1 item if gases diffs have the same amount - will be merged to a single key
      return;
    }

    const auto&[amount1, type1] = *diffs.cbegin();
    const auto&[amount2, type2] = *diffs.crbegin();
    CONTRACT_ASSERT(type1 != type2);

    if (amount1 * amount2 >= 0)
      return;

    const int DiffusionSpeed = 1000; // particles / sec
    const int amountToDiffuse =
      std::min<int>(
      std::min<int>(std::abs(amount1), std::abs(amount2)),
      (int)(DiffusionSpeed * i_dt));

    if (amount1 > 0)
    {
      i_atmo1.giveGas(type1, i_atmo2.tryTakeGas(type1, amountToDiffuse));
      i_atmo2.giveGas(type2, i_atmo1.tryTakeGas(type2, amountToDiffuse));
    }
    else
    {
      i_atmo2.giveGas(type1, i_atmo1.tryTakeGas(type1, amountToDiffuse));
      i_atmo1.giveGas(type2, i_atmo2.tryTakeGas(type2, amountToDiffuse));
    }
  }

  int getGasToSpread(int i_pressureDiff, double i_dt)
  {
    const int totalGasToSpread = i_pressureDiff / 2;

    const double SpreadMultiplier = 10.0;

    int gasToSpread = (int)(static_cast<double>(totalGasToSpread) * i_dt * SpreadMultiplier);
    if (gasToSpread > totalGasToSpread)
      gasToSpread = totalGasToSpread;
    if (gasToSpread == 0)
      gasToSpread = 1;

    return gasToSpread;
  }


  Sdk::Vector4F getGasMixColor(const Atmosphere& i_atmo)
  {
    const std::map<Gas, Sdk::Vector4F> ColorsMap = {
      { Gas::Oxygen, { 0.0f, 0.75f, 1.0f, 1.0f} },
      { Gas::CarbonDioxide, { 1.0f, 1.0f, 0.0f, 1.0f} },
    };

    const auto ratios = i_atmo.getGasesRatios();

    Sdk::Vector4F color{ Sdk::Vector4F::zero() };
    for (const auto&[type, _] : i_atmo.getGases())
      color += ColorsMap.at(type) * (float)ratios.at(type);

    return color;
  }

  float getGasMixIntensity(const Atmosphere& i_atmo)
  {
    const double AveragePressure = 100000.0; // 1atm
    const int pressure = i_atmo.getPressure();

    return (float)(pressure / AveragePressure * 0.5);
  }

} // anonymous NS


void Tile::updateAtmosphere(double i_dt)
{
  const auto pressure1 = d_atmosphere.getPressure();
  if (!hasAtmosphere() && pressure1 == 0)
    return;


  if (hasLeak(d_world, d_coordsTile))
    d_atmosphere.removeAllGases();


  if (!hasAtmosphere())
  {
    if (pressure1 > 0)
      tryPushGasesToNeighbors(d_atmosphere, d_world, d_coordsTile);

    return;
  }


  auto& atmo1 = d_atmosphere;
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

    // TODO: ae Review this condition
    if (pressureDiff < 1000)
      diffuseGases(atmo1, atmo2, i_dt);

    if (pressureDiff <= 1)
      continue;

    const int gasToSpread = getGasToSpread(pressureDiff, i_dt);

    auto& srcAtmo = pressure2 > pressure1 ? atmo2: atmo1;
    auto& destAtmo = pressure2 > pressure1 ? atmo1: atmo2;

    shareGases(srcAtmo, destAtmo, gasToSpread);
  }
}


void Tile::setOverlayColor() const
{
  if (!d_atmosphere.hasGases())
    return;

  Sdk::Vector4F color = getGasMixColor(d_atmosphere);
  color.w = getGasMixIntensity(d_atmosphere);

  d_overlaySprite.setColor(color);
}
