#pragma once

#include "Actions.h"
#include "BehaviorModelBase.h"
#include "Container.h"

#include <cstdint>


class ContainerModel : public BehaviorModelBase
{
public:
  ContainerModel(Structure& io_structure);

  virtual void update(double i_dt) override;

  virtual void interact(Action i_action) override;

private:
  Container d_container;

public:
  virtual void writeTo(std::ostream& io_stream) const override;
  virtual void readFrom(std::istream& io_stream, Dx::IResourceController& i_resouceContoller, World& i_world) override;
};
