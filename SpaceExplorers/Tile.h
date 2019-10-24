#pragma once

#include "Atmosphere.h"
#include "Fwd.h"
#include "OverlayOptions.h"
#include "Structure.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Vector.h>

#include <iostream>
#include <map>


class Tile
{
public:
  Tile(Sdk::Vector2I i_coordsTile, World& i_world, Dx::IResourceController& i_resourceController);

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer, OverlayOption i_overlayOption) const;

  void setStructure(Layer i_layer, StructurePtr i_structure);
  void removeStructure(Layer i_layer);
  void removeStructure(StructurePtr i_structure);

  StructurePtr getTopStructure();
  const StructurePtr getTopStructure() const;

  StructurePtr getStructure(const Sdk::Vector2I& i_relativeCoords) const;

  bool hasStructures() const;
  bool hasStructureOnLayer(Layer i_layer) const;

  bool isPassable() const;
  bool hasAtmosphere() const;
  bool isLeak() const;
  bool isSupport() const;

  Atmosphere& getAtmosphere() { return d_atmosphere; }
  const Atmosphere& getAtmosphere() const { return d_atmosphere; }

private:
  Sdk::Vector2I d_coordsTile;
  World& d_world;
  Dx::IResourceController& d_resourceController;

  std::map<Layer, StructurePtr> d_layersMap;

  Atmosphere d_atmosphere;
  void updateAtmosphere(double i_dt);

  mutable Dx::Sprite d_overlaySprite;
  void setOverlayColor() const;

public:
  void writeTo(std::ostream& io_stream) const;
  void readFrom(std::istream& io_stream);
};
