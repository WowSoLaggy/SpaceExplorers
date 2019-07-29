#pragma once

#include "Fwd.h"
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

  void resetItem(int i_index);
  void setItem(int i_index, const StructurePrototype& i_prototype);
  const StructurePrototype* getItem(int i_index) const;

private:
  const int SlotsHor = 8;
  const int SlotsVert = 1;
  const int SlotsCount = SlotsHor * SlotsVert;
  const int CornerSize = 13;
  const int SlotSize = 72;

  const Dx::IResourceController& d_resourceController;

  Sdk::Vector2I d_position;
  std::vector<Dx::Sprite> d_sprites;

  std::vector<const StructurePrototype*> d_items;
  std::vector<Dx::Sprite> d_itemSprites;

  void checkIndex(int i_index) const;

  void recreateSprites();
  void updateItemSprite(int i_index);
};
