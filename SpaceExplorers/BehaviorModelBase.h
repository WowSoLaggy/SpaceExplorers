#pragma once

#include "Actions.h"
#include "Fwd.h"

#include <LaggyDx/LaggyDxFwd.h>

#include <iostream>


class BehaviorModelBase
{
public:
  BehaviorModelBase(Structure& io_structure, World& io_world);

  virtual void update(double i_dt);

  virtual void interact(Action i_action);
  virtual bool isPassable() const;
  virtual bool hasAtmosphere() const;
  virtual bool isLeak() const;
  virtual bool isSupport() const;

protected:
  Structure& d_structure;
  World& d_world;

public:
  virtual void writeTo(std::ostream& io_stream) const;
  virtual void readFrom(std::istream& io_stream, Dx::IResourceController& i_resouceContoller, World& i_world);
};
