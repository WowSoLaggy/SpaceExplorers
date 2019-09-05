#include "stdafx.h"
#include "BehaviorModelBase.h"

#include "Structure.h"


BehaviorModelBase::BehaviorModelBase(Structure& io_structure)
  : d_structure(io_structure)
{
}


void BehaviorModelBase::update(double i_dt) { }


void BehaviorModelBase::interact(Action i_action) { }

bool BehaviorModelBase::isPassable() const
{
  return d_structure.getPrototype().isPassable;
};


void BehaviorModelBase::writeTo(std::ostream& io_stream) const { }

void BehaviorModelBase::readFrom(std::istream& io_stream) { }
