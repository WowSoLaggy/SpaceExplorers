#pragma once

#include "Fwd.h"
#include "Structure.h"

#include <LaggyDx/LaggyDxFwd.h>

#include <iostream>
#include <map>


class Tile
{
public:
  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

  void setStructure(Layer i_layer, StructurePtr i_structure);
  void removeStructure(Layer i_layer);
  void removeStructure(StructurePtr i_structure);

  StructurePtr getTopStructure();
  const StructurePtr getTopStructure() const;

  bool hasStructures() const;
  bool hasStructureOnLayer(Layer i_layer) const;

private:
  std::map<Layer, StructurePtr> d_layersMap;

public:
  void writeTo(std::ostream& io_stream) const;
  void readFrom(std::istream& io_stream,
                Dx::IResourceController& i_resourceController,
                const Sdk::Vector2I& i_coords);
};
