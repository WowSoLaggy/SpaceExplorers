#pragma once

#include "Object.h"
#include "Structure.h"
#include "Tile.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Rect.h>
#include <LaggySdk/Vector.h>

#include <iostream>
#include <unordered_map>


class World
{
public:
  static std::unique_ptr<World> createEmptyWorld(
    Sdk::Vector2I i_backgroundSize,
    Dx::IResourceController& i_resourceController);

  static std::unique_ptr<World> createTestWorld(
    Sdk::Vector2I i_backgroundSize,
    Dx::IResourceController& i_resourceController);

public:
  World(Dx::IResourceController& i_resourceController);

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer, const Sdk::RectI& i_viewport) const;

  Tile* getTile(const Sdk::Vector2I& i_coords);
  const Tile* getTile(const Sdk::Vector2I& i_coords) const;
  Tile& getOrCreateTile(const Sdk::Vector2I& i_coords);

  void createStructureAt(const StructurePrototype& i_prototype, const Sdk::Vector2I& i_coords);

  void setBackground(const std::string& i_backgroundTextureFilename,
                     Sdk::Vector2I i_backgroundSize);

  Object& getObject(const std::string& i_name) { return *d_objects.at(i_name); }
  const Object& getObject(const std::string& i_name) const { return *d_objects.at(i_name); }
  ObjectPtr& getObjectPtr(const std::string& i_name) { return d_objects.at(i_name); }
  const ObjectPtr& getObjectPtr(const std::string& i_name) const { return d_objects.at(i_name); }

private:
  Dx::IResourceController& d_resourceController;
  Dx::Sprite d_background;

  std::unordered_map<Sdk::Vector2I, Tile, Sdk::Vector2_hash> d_tilesMap;
  std::unordered_map<std::string, ObjectPtr> d_objects;

public:
  void writeTo(std::ostream& io_stream) const;
  static std::unique_ptr<World> readFrom(std::istream& io_stream,
                                         Sdk::Vector2I i_backgroundSize,
                                         Dx::IResourceController& i_resourceController);
};
