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

  virtual void render(Dx::IRenderer2d& i_renderer) const override;

  void setPosition(Sdk::Vector2I i_position) { d_position = std::move(i_position); }
  virtual Sdk::Vector2I getSize() const override;

  void resetItem(int i_index);
  void setItem(int i_index, const StructurePrototype& i_prototype);
  const StructurePrototype* getItem(int i_index) const;

  void selectItem(int i_index);
  void unselectItem();
  std::optional<int> getSelectedIndex() const;
  const StructurePrototype* getSelectedItem() const;
  bool hasSelection() const;

private:
  static const int SlotsHor = 8;
  static const int SlotsVert = 1;
  static const int SlotsCount = SlotsHor * SlotsVert;
  static const int CornerSize = 13;
  static const int SlotSize = 72;
  static void CheckIndex(int i_index);

  const Dx::IResourceController& d_resourceController;

  Sdk::Vector2I d_position;
  std::vector<Dx::Sprite> d_gridSprites;

  std::vector<const StructurePrototype*> d_items;
  std::vector<Dx::Sprite> d_itemSprites;
  std::optional<int> d_selectedIndex;
  Dx::Sprite d_selectionSprite;

  void recreateSprites();
  void updateItemSprite(int i_index);
  void updateSelectionSprite();
};
