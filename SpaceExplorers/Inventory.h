#pragma once

#include "Fwd.h"
#include "IGuiControl.h"
#include "Object.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/EventHandler.h>
#include <LaggySdk/Vector.h>

#include <vector>


class Inventory : public IGuiControl, public Sdk::EventHandler
{
public:
  Inventory(Dx::IResourceController& i_resourceController, Container& io_container,
            int i_sizeX, int i_sizeY);

  virtual void render(Dx::IRenderer2d& i_renderer) const override;
  virtual void processEvent(const Sdk::IEvent& i_event) override;

  void setPosition(Sdk::Vector2I i_position) { d_position = std::move(i_position); }
  virtual Sdk::Vector2I getSize() const override;

  void selectItem(int i_index);
  void unselectItem();
  std::optional<int> getSelectedIndex() const;
  ObjectPtr getSelectedItem() const;
  bool hasSelection() const;

private:
  const int d_slotsHor = 0;
  const int d_slotsVert = 0;
  const int d_slotsCount = 0;
  static const int CornerSize = 13;
  static const int SlotSize = 72;
  void checkIndex(int i_index) const;

  Dx::IResourceController& d_resourceController;
  const Dx::IFontResource& d_fontResource;
  Container& d_container;

  Sdk::Vector2I d_position;
  std::vector<Dx::Sprite> d_gridSprites;

  std::vector<Dx::Sprite> d_itemSprites;
  std::optional<int> d_selectedIndex;
  Dx::Sprite d_selectionSprite;

  void recreateSprites();
  void updateItemSprite(int i_index);
  void updateSelectionSprite();
};
