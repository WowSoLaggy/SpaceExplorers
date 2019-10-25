#include "stdafx.h"
#include "Tile.h"

#include "SettingsProvider.h"
#include "World.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>
#include <LaggySdk/Contracts.h>


Tile::Tile(Sdk::Vector2I i_coordsTile, World& i_world, Dx::IResourceController& i_resourceController)
  : d_coordsTile(i_coordsTile)
  , d_world(i_world)
  , d_resourceController(i_resourceController)
{
  d_overlaySprite.setTexture(&d_resourceController.getTextureResource("White.png"));

  auto tileSize = SettingsProvider::getDefaultInternalSettings().tileSize;
  d_overlaySprite.setPosition(d_coordsTile * tileSize);

  d_overlaySprite.setSize({ tileSize, tileSize });
}


void Tile::update(double i_dt)
{
  for (auto& [_, structure] : d_layersMap)
  {
    CONTRACT_EXPECT(structure);
    structure->update(i_dt);
  }

  updateAtmosphere(i_dt);
}

void Tile::render(Dx::IRenderer2d& i_renderer, OverlayOption i_overlayOption) const
{
  if (d_layersMap.empty())
    return;

  auto it = std::prev(d_layersMap.cend());
  while (it != d_layersMap.cbegin() && it->second->isTransparent())
    --it;

  for (; it != d_layersMap.cend(); ++it)
    it->second->render(i_renderer);

  if (i_overlayOption == OverlayOption::Atmosphere && d_atmosphere.hasGases())
  {
    setOverlayColor();
    i_renderer.renderSprite(d_overlaySprite);
  }
}


void Tile::setStructure(Layer i_layer, StructurePtr i_structure)
{
  CONTRACT_EXPECT(i_layer != Layer::None);
  CONTRACT_EXPECT(i_structure);
  d_layersMap[i_layer] = i_structure;
}

void Tile::removeStructure(Layer i_layer)
{
  CONTRACT_EXPECT(hasStructureOnLayer(i_layer));
  d_layersMap.erase(i_layer);
}

void Tile::removeStructure(StructurePtr i_structure)
{
  d_layersMap.erase(std::find_if(d_layersMap.begin(), d_layersMap.end(),
                    [&](const auto& i_pair) { return i_pair.second == i_structure; }));
}


StructurePtr Tile::getTopStructure()
{
  if (d_layersMap.empty())
    return nullptr;
  
  auto structure = d_layersMap.rbegin()->second;
  CONTRACT_EXPECT(structure);
  return structure;
}

const StructurePtr Tile::getTopStructure() const
{
  if (d_layersMap.empty())
    return nullptr;

  auto structure = d_layersMap.crbegin()->second;
  CONTRACT_ASSERT(structure);
  return structure;
}


StructurePtr Tile::getStructure(const Sdk::Vector2I& i_relativeCoords) const
{
  for (auto it = d_layersMap.crbegin(); it != d_layersMap.crend(); ++it)
  {
    auto structure = it->second;
    CONTRACT_ASSERT(structure);
    if (structure->checkAlpha(i_relativeCoords))
      return structure;
  }

  return nullptr;
}


bool Tile::hasStructures() const
{
  return !d_layersMap.empty();
}

bool Tile::hasStructureOnLayer(Layer i_layer) const
{
  return d_layersMap.find(i_layer) != d_layersMap.cend();
}


bool Tile::isPassable() const
{
  if (const auto topStructure = getTopStructure())
    return topStructure->isPassable();
  return true;
}

bool Tile::hasAtmosphere() const
{
  return std::all_of(d_layersMap.begin(), d_layersMap.end(), [](const auto& pair)
  {
    return pair.second->hasAtmosphere();
  });
}

bool Tile::isLeak() const
{
  return std::all_of(d_layersMap.begin(), d_layersMap.end(), [](const auto& pair)
  {
    return pair.second->isLeak();
  });
}

bool Tile::isSupport() const
{
  return std::any_of(d_layersMap.begin(), d_layersMap.end(), [](const auto& pair)
  {
    return pair.second->isSupport();
  });
}


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
      gases1.clear();
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
        for (auto&[type, amount] : gases1)
        {
          const auto gasToGive = amount / tileCountLeft;
          tile->getAtmosphere().d_gases[type] += gasToGive;
          amount -= gasToGive;
        }
        --tileCountLeft;
      }

      // If some gases still left (due to rounding error) -
      // just give it to the first tile
      if (tileCount > 0)
      {
        for (auto&[type, amount] : gases1)
          tiles.front()->getAtmosphere().d_gases[type] += amount;

        gases1.clear();
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
    auto& gases2 = atmo2.getGases();
    
    const auto pressure2 = atmo2.getPressure();
    if (pressure1 == 0 && pressure2 == 0)
      continue;

    const int pressureDiff = pressure2 - pressure1;
    if (std::abs(pressureDiff) <= 1)
      continue;

    const int totalGasToSpread = pressureDiff / 2;

    const double SpreadMultiplier = 5.0;

    int gasToSpread = (int)(static_cast<double>(totalGasToSpread) * i_dt * SpreadMultiplier);
    if (gasToSpread > totalGasToSpread)
      gasToSpread = totalGasToSpread;
    if (gasToSpread == 0)
      gasToSpread = pressure2 - pressure1 > 0 ? 1 : -1;

    gases1[Gas::Oxygen] += gasToSpread;
    gases2[Gas::Oxygen] -= gasToSpread;
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

  Sdk::Vector4F color{ Sdk::Vector4F::zero() };
  for (const auto&[type, _] : d_atmosphere.getGases())
    color += ColorsMap.at(type);

  float gasCount = (float)d_atmosphere.getGases().size();
  color.x = color.x / gasCount;
  color.y = color.y / gasCount;
  color.z = color.z / gasCount;

  const double AveragePressure = 100000.0; // 1atm
  const int pressure = d_atmosphere.getPressure();

  const double intensity = pressure / AveragePressure * 0.5;
  color.w = (float)intensity;

  d_overlaySprite.setColor(color);
}
