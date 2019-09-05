#pragma once

#include "Structure.h"

#include <LaggyDx/LaggyDxFwd.h>

#include <cstdint>


class Door : public Structure
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
  Door(
    Dx::IResourceController& i_resourceController,
    const StructurePrototype& i_prototype,
    Sdk::Vector2I i_coordsTile);

  virtual void update(double i_dt) override;

  virtual void interact(Action i_action) override;
  virtual bool isPassable() const override;

  void setState(State i_state);

private:
  State d_state = State::Closed;

  Action getDefaultAction() const;

public:
  virtual void writeTo(std::ostream& io_stream) const override;
  virtual void readFrom(std::istream& io_stream) override;
};
