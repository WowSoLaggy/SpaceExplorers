#include "stdafx.h"
#include "World.h"

#include "Prototypes.h"
#include "Structure.h"
#include "SettingsProvider.h"
#include "Utils.h"

#include <LaggyDx/ITextureResource.h>
#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>


World::World(Dx::IResourceController& i_resourceController)
  : d_resourceController(i_resourceController)
{
}


void World::update(double i_dt)
{
  for (auto&[_, tile] : d_tilesMap)
    tile.update(i_dt);

  for (auto& object : d_objects)
    object->update(i_dt);

  for (auto& [_, avatar] : d_avatars)
    avatar->update(i_dt);
}

void World::render(Dx::IRenderer2d& i_renderer, const Sdk::RectI& i_viewport, OverlayOption i_overlayOption) const
{
  i_renderer.resetTranslation();
  i_renderer.renderSprite(d_background);

  i_renderer.setTranslation(i_viewport.topLeft());
  for (const auto&[coords, tile] : d_tilesMap)
  {
    const auto tileRect = getTileRect(coords);
    if (i_viewport.intersectRect(tileRect))
      tile.render(i_renderer, i_overlayOption);
  }

  for (auto& object : d_objects)
  {
    if (i_viewport.intersectRect(object->getRect()))
      object->render(i_renderer);
  }

  for (auto& [_, avatar] : d_avatars)
  {
    if (i_viewport.intersectRect(avatar->getRect()))
      avatar->render(i_renderer);
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
  Tile tile(i_coords, *this, d_resourceController);
  auto pair = d_tilesMap.insert({ i_coords, std::move(tile) });
  return pair.first->second;
}


StructurePtr World::createStructureAt(const StructurePrototype& i_prototype, const Sdk::Vector2I& i_coords)
{
  StructurePtr structure = std::make_shared<Structure>(d_resourceController, *this, i_prototype, i_coords);

  auto& tile = getOrCreateTile(i_coords);
  tile.setStructure(i_prototype.layer, structure);

  return structure;
}

ObjectPtr World::createObjectAt(const ObjectPrototype& i_prototype, Sdk::Vector2I i_coords, std::string i_name /*= ""*/)
{
  auto objectPtr = std::make_shared<Object>(d_resourceController, *this, i_prototype);
  objectPtr->setPosition(std::move(i_coords));

  objectPtr->setName(!i_name.empty() ? i_name : i_prototype.name);

  d_objects.push_back(objectPtr);

  return objectPtr;
}

AvatarPtr World::createAvatarAt(const ObjectPrototype& i_prototype, Sdk::Vector2I i_coords, std::string i_name)
{
  auto avatarPtr = std::make_shared<Avatar>(d_resourceController, *this, i_prototype);
  avatarPtr->setPosition(std::move(i_coords));
  avatarPtr->setName(std::move(i_name));
  
  d_avatars.insert({ avatarPtr->getName(), avatarPtr });

  return avatarPtr;
}

bool World::removeStructureAtLayer(const Sdk::Vector2I& i_coords, Layer i_layer)
{
  if (auto* tile = getTile(i_coords))
  {
    tile->removeStructure(i_layer);
    return true;
  }
  return false;
}


void World::setBackground(const std::string& i_backgroundTextureFilename,
                          Sdk::Vector2I i_backgroundSize)
{
  const Dx::ITextureResource* texture = &d_resourceController.getTextureResource(i_backgroundTextureFilename);
  d_background.setTexture(texture);
  d_background.setSize(std::move(i_backgroundSize));
}


StructurePtr World::getStructureAt(const Sdk::Vector2I& i_coords) const
{
  const auto tileCoords = worldToTile(i_coords);

  const auto* tile = getTile(tileCoords);
  if (!tile)
    return nullptr;

  return tile->getStructure(i_coords - getTileRect(tileCoords).topLeft());
}

ObjectPtr World::getObjectAt(const Sdk::Vector2I& i_coords) const
{
  for (const auto& [_, avatar] : d_avatars)
  {
    const auto& rect = avatar->getRect();
    if (!rect.containsPoint(i_coords))
      continue;

    const auto relativeCoords = i_coords - rect.topLeft();
    if (avatar->checkAlpha(relativeCoords))
      return avatar;
  }

  for (const auto& obj : d_objects)
  {
    const auto& rect = obj->getRect();
    if (!rect.containsPoint(i_coords))
      continue;

    const auto relativeCoords = i_coords - rect.topLeft();
    if (obj->checkAlpha(relativeCoords))
      return obj;
  }

  return nullptr;
}

void World::deleteObject(ObjectPtr i_object)
{
  d_objects.erase(std::find(d_objects.cbegin(), d_objects.cend(), i_object));
}

void World::deleteObject(const Object& i_object)
{
  d_objects.erase(std::find_if(d_objects.cbegin(), d_objects.cend(), [&](ObjectPtr i_right) {
    return i_right.get() == &i_object;
  }));
}


bool World::checkCollision(const Sdk::RectI& i_rect) const
{
  auto isPassable = [&](const Sdk::Vector2I& i_tileCoords) -> bool
  {
    if (const auto* pTile = getTile(i_tileCoords))
      return pTile->isPassable();
    return true;
  };

  std::unordered_set<Sdk::Vector2I, Sdk::Vector2_hash> tilesToCheck;
  tilesToCheck.insert(worldToTile(i_rect.topLeft()));
  tilesToCheck.insert(worldToTile(i_rect.topRight()));
  tilesToCheck.insert(worldToTile(i_rect.bottomLeft()));
  tilesToCheck.insert(worldToTile(i_rect.bottomRight()));

  return !std::all_of(tilesToCheck.cbegin(), tilesToCheck.cend(), [&](const Sdk::Vector2I& i_tileCoords) {
    return isPassable(i_tileCoords);
  });
}

bool World::checkSupport(const Sdk::RectI& i_rect) const
{
  auto hasSupport = [&](const Sdk::Vector2I& i_tileCoords) -> bool
  {
    if (const auto* pTile = getTile(i_tileCoords))
      return pTile->isSupport();
    return false;
  };

  std::unordered_set<Sdk::Vector2I, Sdk::Vector2_hash> tilesToCheck;
  tilesToCheck.insert(worldToTile(i_rect.topLeft()));
  tilesToCheck.insert(worldToTile(i_rect.topRight()));
  tilesToCheck.insert(worldToTile(i_rect.bottomLeft()));
  tilesToCheck.insert(worldToTile(i_rect.bottomRight()));

  return std::any_of(tilesToCheck.cbegin(), tilesToCheck.cend(), [&](const Sdk::Vector2I& i_tileCoords) {
    return hasSupport(i_tileCoords);
  });
}

bool World::checkSupport(const Sdk::Vector2I& i_tileCoords) const
{
  auto hasSupport = [&](const Sdk::Vector2I& i_tileCoords) -> bool
  {
    if (const auto* pTile = getTile(i_tileCoords))
      return pTile->isSupport();
    return false;
  };

  std::unordered_set<Sdk::Vector2I, Sdk::Vector2_hash> tilesToCheck;
  tilesToCheck.insert(i_tileCoords);
  tilesToCheck.insert(i_tileCoords + Sdk::Vector2I{ 1, 0 });
  tilesToCheck.insert(i_tileCoords + Sdk::Vector2I{ -1, 0 });
  tilesToCheck.insert(i_tileCoords + Sdk::Vector2I{ 0, 1 });
  tilesToCheck.insert(i_tileCoords + Sdk::Vector2I{ 0, -1 });

  return std::any_of(tilesToCheck.cbegin(), tilesToCheck.cend(), [&](const Sdk::Vector2I& i_tileCoords) {
    return hasSupport(i_tileCoords);
  });
}

bool World::checkIntersectWithAnyObject(const Sdk::RectI& i_rect) const
{
  for (const auto& [_, avatar] : d_avatars)
  {
    auto rect = avatar->getRect();
    rect.shrink(CollisionShrink + 1);
    if (rect.intersectRect(i_rect))
      return true;
  }

  for (const auto& obj : d_objects)
  {
    auto rect = obj->getRect();
    rect.shrink(CollisionShrink);
    if (rect.intersectRect(i_rect))
      return true;
  }

  return false;
}
