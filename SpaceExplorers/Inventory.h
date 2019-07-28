#pragma once

#include "IGuiControl.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Vector.h>

#include <vector>


class Inventory : public IGuiControl
{
public:
  Inventory(const Dx::IResourceController& i_resourceController);

  void update(double i_dt) override;
  void render(Dx::IRenderer2d& i_renderer) const override;

  void setPosition(Sdk::Vector2I i_position) { d_position = std::move(i_position); }
  Sdk::Vector2I getSize() const;

private:
  const int SlotsHor = 8;
  const int SlotsVert = 1;
  const int CornerSize = 13;
  const int SlotSize = 72;

  const Dx::IResourceController& d_resourceController;

  Sdk::Vector2I d_position;
  std::vector<Dx::Sprite> d_sprites;

  void recreateSprites();
};
