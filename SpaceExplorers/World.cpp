#include "stdafx.h"
#include "World.h"

#include "Door.h"
#include "Prototypes.h"
#include "Structure.h"
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


World::World(Dx::IResourceController& i_resourceController)
  : d_resourceController(i_resourceController)
{
}


void World::update(double i_dt)
{
  for (auto&[_, tile] : d_tilesMap)
    tile.update(i_dt);

  for (auto& [_, object] : d_objects)
    object->update(i_dt);
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

  for (auto& [_, object] : d_objects)
  {
    if (i_viewport.intersectRect(object->getRect()))
      object->render(i_renderer);
  }
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


void World::createStructureAt(const StructurePrototype& i_prototype, const Sdk::Vector2I& i_coords)
{
  StructurePtr structure;

  if (i_prototype.behavior == Behavior::Door)
    structure = std::make_shared<Door>(d_resourceController, i_prototype, i_coords);
  else 
    structure = std::make_shared<Structure>(d_resourceController, i_prototype, i_coords);

  d_tilesMap[i_coords].setStructure(i_prototype.layer, structure);
}

void World::createObjectAt(const ObjectPrototype& i_prototype, Sdk::Vector2I i_coords)
{
  auto objectPtr = std::make_shared<Object>(d_resourceController, *this, i_prototype);
  objectPtr->setPosition(std::move(i_coords));

  d_objects.insert({ objectPtr->getName(), std::move(objectPtr) });
}

void World::createAvatarAt(const ObjectPrototype& i_prototype, Sdk::Vector2I i_coords, std::string i_name)
{
  auto objectPtr = std::make_shared<Object>(d_resourceController, *this, i_prototype);
  objectPtr->setPosition(std::move(i_coords));
  objectPtr->setName(std::move(i_name));
  
  d_objects.insert({ objectPtr->getName(), std::move(objectPtr) });
}


void World::setBackground(const std::string& i_backgroundTextureFilename,
                          Sdk::Vector2I i_backgroundSize)
{
  const Dx::ITextureResource* texture = &d_resourceController.getTextureResource(i_backgroundTextureFilename);
  d_background.setTexture(texture);
  d_background.setSize(std::move(i_backgroundSize));
}
