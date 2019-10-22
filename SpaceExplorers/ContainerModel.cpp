#include "stdafx.h"
#include "ContainerModel.h"

#include "Structure.h"


ContainerModel::ContainerModel(Structure& io_structure)
  : BehaviorModelBase(io_structure)
{
  d_container.setSize(io_structure.getPrototype().containerSize);
}


void ContainerModel::update(double i_dt)
{
}


void ContainerModel::interact(Action i_action)
{
  if (i_action == Action::Open)
  {
    d_structure.getSprite().playAnimation("Open");
  }
  else if (i_action == Action::Close)
  {
    d_structure.getSprite().playAnimation("Close");
  }
}


void ContainerModel::writeTo(std::ostream& io_stream) const
{
  d_container.writeTo(io_stream);
}

void ContainerModel::readFrom(std::istream& io_stream, Dx::IResourceController& i_resouceContoller, World& i_world)
{
  d_container.readFrom(io_stream, i_resouceContoller, i_world);
}
