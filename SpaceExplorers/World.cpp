#include "stdafx.h"
#include "World.h"

#include "SettingsProvider.h"

#include <LaggyDx/ITextureResource.h>
#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>


namespace
{
  Sdk::RectI getTileRect(const Sdk::Vector2I& i_coords)
  {
    const int tileSize = SettingsProvider::getDefaultInternalSettings().tileSize;

    const Sdk::Vector2I topLeft = i_coords * tileSize;
    const Sdk::Vector2I bottomRight = topLeft + Sdk::Vector2I{ tileSize, tileSize };

    return Sdk::RectI{ topLeft, bottomRight};
  }
} // anonymous NS

void World::update(double i_dt)
{
  for (auto&[_, tile] : d_tilesMap)
    tile.update(i_dt);

  /*for (auto& item : d_objects)
    item->update(i_dt);*/
}

void World::render(Dx::IRenderer2d& i_renderer, const Sdk::RectI& i_viewport) const
{
  i_renderer.resetTranslation();
  i_renderer.renderSprite(d_background);

  i_renderer.setTranslation(i_viewport.topLeft());
  for (const auto&[coords, tile] : d_tilesMap)
  {
    const auto tileRect = getTileRect(coords);
    if (i_viewport.intersectRect(tileRect))
      tile.render(i_renderer);
  }

  /*for (const auto& item : d_objects)
    item->render(i_renderer);*/
}


Tile* World::getTile(const Sdk::Vector2I& i_coords)
{
  if (auto it = d_tilesMap.find(i_coords); it != d_tilesMap.end())
    return &it->second;

  return nullptr;
}

const Tile* World::getTile(const Sdk::Vector2I& i_coords) const
{
  if (auto it = d_tilesMap.find(i_coords); it != d_tilesMap.end())
    return &it->second;

  return nullptr;
}

Tile& World::getOrCreateTile(const Sdk::Vector2I& i_coords)
{
  return d_tilesMap[i_coords];
}


void World::setBackground(const std::string& i_backgroundTextureFilename,
                          Sdk::Vector2I i_backgroundSize,
                          const Dx::IResourceController& i_resourceController)
{
  const Dx::ITextureResource* texture = &i_resourceController.getTextureResource(i_backgroundTextureFilename);
  d_background.setTexture(texture);
  d_background.setSize(std::move(i_backgroundSize));
}
