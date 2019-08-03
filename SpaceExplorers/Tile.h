#pragma once

#include "Structure.h"

#include <LaggyDx/LaggyDxFwd.h>

#include <map>


class Tile
{
public:
  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

  void setStructure(Layer i_layer, StructurePtr i_structure);
  void removeStructure(Layer i_layer);

  StructurePtr getTopStructure();
  const StructurePtr getTopStructure() const;

  bool hasStructures() const;
  bool hasStructureOnLayer(Layer i_layer) const;

private:
  std::map<Layer, StructurePtr> d_layersMap;
};
