#pragma once

#include "Structure.h"

#include <LaggyDx/LaggyDxFwd.h>


class Door : public Structure
{
public:
  Door(
    const Dx::IResourceController& i_resourceController,
    const StructurePrototype& i_prototype,
    Sdk::Vector2I i_coordsTile);

  virtual void update(double i_dt) override;

  virtual void interact(Actions i_action) override;

private:
  Actions getDefaultAction() const;
  Dx::AnimatedSprite& getAnimatedSprite();
};