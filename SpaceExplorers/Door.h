#pragma once

#include "Structure.h"

#include <LaggyDx/LaggyDxFwd.h>

#include <cstdint>


class Door : public Structure
{
public:
  Door(
    Dx::IResourceController& i_resourceController,
    const StructurePrototype& i_prototype,
    Sdk::Vector2I i_coordsTile);

  virtual void update(double i_dt) override;

  virtual void interact(Actions i_action) override;

private:
  enum class State : std::int32_t
  {
    Open = 0,
    Closed,
    Opening,
    Closing,
  };

  State d_state = State::Closed;

  Actions getDefaultAction() const;
  Dx::AnimatedSprite& getAnimatedSprite();
  const Dx::AnimatedSprite& getAnimatedSprite() const;

protected:
  virtual std::ostream& write(std::ostream& io_stream) const override;
};
