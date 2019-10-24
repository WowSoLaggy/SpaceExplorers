#include "stdafx.h"
#include "BehaviorModelBase.h"

#include "Structure.h"


BehaviorModelBase::BehaviorModelBase(Structure& io_structure, World& io_world)
  : d_structure(io_structure)
  , d_world(io_world)
{
}


void BehaviorModelBase::update(double i_dt) { }


void BehaviorModelBase::interact(Action i_action) { }

bool BehaviorModelBase::isPassable() const
{
  return d_structure.getPrototype().isPassable;
}

bool BehaviorModelBase::hasAtmosphere() const
{
  return d_structure.getPrototype().hasAtmosphere;
}

bool BehaviorModelBase::isLeak() const
{
  return d_structure.getPrototype().isLeak;
}

bool BehaviorModelBase::isSupport() const
{
  return d_structure.getPrototype().isSupport;
}


void BehaviorModelBase::writeTo(std::ostream& io_stream) const { }

void BehaviorModelBase::readFrom(std::istream& io_stream, Dx::IResourceController& i_resouceContoller, World& i_world) { }
