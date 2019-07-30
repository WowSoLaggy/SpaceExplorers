#pragma once

#include "Object.h"
#include "Structure.h"
#include "Tile.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Rect.h>
#include <LaggySdk/Vector.h>

#include <unordered_map>
#include <vector>


class World
{
public:
  static std::unique_ptr<World> createTestWorld(
    Sdk::Vector2I i_backgroundSize,
    const Dx::IResourceController& i_resourceController);

public:
  World(const Dx::IResourceController& i_resourceController);

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer, const Sdk::RectI& i_viewport) const;

  Tile* getTile(const Sdk::Vector2I& i_coords);
  const Tile* getTile(const Sdk::Vector2I& i_coords) const;
  Tile& getOrCreateTile(const Sdk::Vector2I& i_coords);

  void createStructureAt(const StructurePrototype& i_prototype, const Sdk::Vector2I& i_coords);

  void setBackground(const std::string& i_backgroundTextureFilename,
                     Sdk::Vector2I i_backgroundSize);

private:
  const Dx::IResourceController& d_resourceController;
  Dx::Sprite d_background;

  std::unordered_map<Sdk::Vector2I, Tile, Sdk::Vector2_hash> d_tilesMap;
  std::vector<ObjectPtr> d_objects;
};
