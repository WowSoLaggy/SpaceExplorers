#pragma once

#include "BehaviorModelBase.h"


class DoorModel : public BehaviorModelBase
{
public:
  enum class State : std::int32_t
  {
    Open = 0,
    Closed,
    Opening,
    Closing,
  };

public:
  DoorModel(Structure& io_structure, World& io_world);

  virtual void update(double i_dt) override;

  virtual void interact(Action i_action) override;
  virtual bool isPassable() const override;
  virtual bool hasAtmosphere() const override;

  void setState(State i_state);

private:
  State d_state = State::Closed;

  Action getDefaultAction() const;

  void onClose();

public:
  virtual void writeTo(std::ostream& io_stream) const override;
  virtual void readFrom(std::istream& io_stream, Dx::IResourceController& i_resouceContoller, World& i_world) override;
};
