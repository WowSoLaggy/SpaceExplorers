#include "stdafx.h"
#include "Tile.h"

#include <LaggySdk/Contracts.h>


Tile::Tile(Sdk::Vector2I i_coordsTile, Dx::IResourceController& i_resourceController)
  : d_coordsTile(i_coordsTile)
  , d_resourceController(i_resourceController)
{
}


void Tile::update(double i_dt)
{
  for (auto& [_, structure] : d_layersMap)
  {
    CONTRACT_EXPECT(structure);
    structure->update(i_dt);
  }
}

void Tile::render(Dx::IRenderer2d& i_renderer) const
{
  if (d_layersMap.empty())
    return;

  auto it = std::prev(d_layersMap.cend());
  while (it != d_layersMap.cbegin() && it->second->isTransparent())
    --it;

  for (; it != d_layersMap.cend(); ++it)
    it->second->render(i_renderer);
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

bool Tile::isSupport() const
{
  return std::any_of(d_layersMap.begin(), d_layersMap.end(), [](const auto& pair)
  {
    return pair.second->isSupport();
  });
}
