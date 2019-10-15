#include "stdafx.h"
#include "Inventory.h"

#include "Container.h"
#include "Container_events.h"
#include "Inventory_events.h"
#include "Prototypes.h"
#include "SettingsProvider.h"

#include <LaggyDx/ImageDescription.h>
#include <LaggyDx/ITextureResource.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/IRenderer2d.h>
#include <LaggySdk/Contracts.h>


Inventory::Inventory(Dx::IResourceController& i_resourceController, Container& io_container,
                     int i_sizeX, int i_sizeY)
  : d_resourceController(i_resourceController)
  , d_fontResource(i_resourceController.getFontResource(SettingsProvider::getDefaultInternalSettings().defaultFontName))
  , d_container(io_container)
  , d_slotsHor(i_sizeX)
  , d_slotsVert(i_sizeY)
  , d_slotsCount(i_sizeX * i_sizeY)
{
  CONTRACT_EXPECT(d_container.getSize() == d_slotsCount);
  connectTo(d_container);
  recreateSprites();
}


void Inventory::render(Dx::IRenderer2d& i_renderer) const
{
  const auto initTranslation = i_renderer.getTranslation();
  i_renderer.setTranslation(-d_position);

  // Grid
  for (const auto& sprite : d_gridSprites)
    i_renderer.renderSprite(sprite);

  // Frames
  for (const auto& sprite : d_framesSprites)
    i_renderer.renderSprite(sprite);

  // Items
  for (int itemIdx = 0; itemIdx < (int)d_itemSprites.size(); ++itemIdx)
  //for (const auto& sprite : d_itemSprites)
  {
    const auto curTranslation = i_renderer.getTranslation();
    i_renderer.setTranslation(curTranslation - d_itemOffsets.at(itemIdx));
    i_renderer.renderSprite(d_itemSprites.at(itemIdx));
    i_renderer.setTranslation(curTranslation);
  }

  // Selection
  if (hasSelection())
    i_renderer.renderSprite(d_selectionSprite);

  // Quantity
  for (int i = 0; i < d_slotsCount; ++i)
  {
    if (const auto item = d_container.getItem(i); item && item->getPrototype().isStackable)
      i_renderer.renderText(std::to_string(item->getQuantity()), d_fontResource, d_itemSprites[i].getPosition());
  }


  i_renderer.setTranslation(initTranslation);
}


void Inventory::processEvent(const Sdk::IEvent& i_event)
{
  if (const auto* event = dynamic_cast<const SlotChangedEvent*>(&i_event))
    updateItemSprite(event->getIndex());
}


Sdk::Vector2I Inventory::getSize() const
{
  return { CornerSize * 2 + d_slotsHor * SlotSize, CornerSize * 2 + d_slotsVert * SlotSize };
}


void Inventory::recreateSprites()
{
  const auto& textureTl(d_resourceController.getTextureResource("GridTl.png"));
  const auto& textureTr(d_resourceController.getTextureResource("GridTr.png"));
  const auto& textureBl(d_resourceController.getTextureResource("GridBl.png"));
  const auto& textureBr(d_resourceController.getTextureResource("GridBr.png"));
  const auto& textureT(d_resourceController.getTextureResource("GridT.png"));
  const auto& textureB(d_resourceController.getTextureResource("GridB.png"));
  const auto& textureL(d_resourceController.getTextureResource("GridL.png"));
  const auto& textureR(d_resourceController.getTextureResource("GridR.png"));
  const auto& textureItem(d_resourceController.getTextureResource("GridItem.png"));
  const auto& textureFrame(d_resourceController.getTextureResource("GridFrame.png"));

  d_gridSprites.emplace_back(Dx::Sprite{
    &textureTl, { 0, 0 },
    textureTl.getDescription().size(), Sdk::Vector4F::identity() });
  d_gridSprites.emplace_back(Dx::Sprite{
    &textureTr, { CornerSize + d_slotsHor * SlotSize, 0 },
    textureTr.getDescription().size(), Sdk::Vector4F::identity() });
  d_gridSprites.emplace_back(Dx::Sprite{
    &textureBl, { 0, CornerSize + d_slotsVert * SlotSize },
    textureBl.getDescription().size(), Sdk::Vector4F::identity() });
  d_gridSprites.emplace_back(Dx::Sprite{
    &textureBr, { CornerSize + d_slotsHor * SlotSize, CornerSize + d_slotsVert * SlotSize },
    textureBr.getDescription().size(), Sdk::Vector4F::identity() });

  for (int i = 0; i < d_slotsHor; ++i)
  {
    d_gridSprites.emplace_back(Dx::Sprite{
      &textureT, { CornerSize + SlotSize * i, 0 },
      textureT.getDescription().size(), Sdk::Vector4F::identity() });
    d_gridSprites.emplace_back(Dx::Sprite{
      &textureB, { CornerSize + SlotSize * i, CornerSize + d_slotsVert * SlotSize },
      textureB.getDescription().size(), Sdk::Vector4F::identity() });
  }

  for (int i = 0; i < d_slotsVert; ++i)
  {
    d_gridSprites.emplace_back(Dx::Sprite{
      &textureL, { 0, CornerSize + SlotSize * i },
      textureL.getDescription().size(), Sdk::Vector4F::identity() });
    d_gridSprites.emplace_back(Dx::Sprite{
      &textureR, { CornerSize + SlotSize * d_slotsHor, CornerSize + SlotSize * i },
      textureR.getDescription().size(), Sdk::Vector4F::identity() });
  }

  for (int y = 0; y < d_slotsVert; ++y)
  {
    for (int x = 0; x < d_slotsHor; ++x)
    {
      d_framesSprites.emplace_back(Dx::Sprite{
        &textureItem, { CornerSize + SlotSize * x, CornerSize + SlotSize * y },
        textureItem.getDescription().size(), Sdk::Vector4F::identity() });

      d_itemSprites.emplace_back(Dx::Sprite{
        nullptr, { CornerSize + SlotSize * x + 4, CornerSize + SlotSize * y + 4 },
        textureItem.getDescription().size(), Sdk::Vector4F::identity() });

      d_itemOffsets.emplace_back(Sdk::Vector2I{ 0, 0 });

      updateItemSprite(x + y * d_slotsHor);
    }
  }

  d_selectionSprite = {
    &textureFrame, { CornerSize + 2, CornerSize + 2 },
    textureFrame.getDescription().size(), Sdk::Vector4F::identity() };
}


void Inventory::checkIndex(int i_index) const
{
  CONTRACT_EXPECT(0 <= i_index);
  CONTRACT_EXPECT(i_index < d_slotsCount);
}


void Inventory::updateItemSprite(int i_index)
{
  checkIndex(i_index);

  auto item = d_container.getItem(i_index);
  const Dx::ITextureResource* texture = item ?
    &d_resourceController.getTextureResource(item->getPrototype().textureFileName) : nullptr;

  if (texture)
  {
    const int adaptedSlotSize = SlotSize - 8;
    d_itemOffsets[i_index] = (Sdk::Vector2I{ adaptedSlotSize, adaptedSlotSize } - texture->getDescription().size()) / 2;
  }
  else
    d_itemOffsets[i_index] = { 0, 0 };

  d_itemSprites.at(i_index).setTexture(texture);
}


void Inventory::selectItem(int i_index)
{
  checkIndex(i_index);

  d_selectedIndex = i_index;
  updateSelectionSprite();
}

void Inventory::unselectItem()
{
  d_selectedIndex = std::nullopt;
}

std::optional<int> Inventory::getSelectedIndex() const
{
  return d_selectedIndex;
}

ObjectPtr Inventory::getSelectedItem() const
{
  if (!d_selectedIndex)
    return nullptr;
  return d_container.getItem(*d_selectedIndex);
}

bool Inventory::hasSelection() const
{
  return d_selectedIndex.has_value();
}

void Inventory::updateSelectionSprite()
{
  if (!d_selectedIndex)
    return;

  int x = *d_selectedIndex % d_slotsHor;
  int y = *d_selectedIndex / d_slotsHor;
  d_selectionSprite.setPosition({ CornerSize + SlotSize * x + 2, CornerSize + SlotSize * y + 2 });
}


bool Inventory::onMouseClick(Dx::MouseKey i_button, const Sdk::Vector2I& i_mousePos)
{
  if (i_button != Dx::MouseKey::Left)
    return false;

  const auto relativePos = i_mousePos - d_position;

  for (int idx = 0; idx < d_slotsCount; ++idx)
  {
    if (d_framesSprites.at(idx).getRect().containsPoint(relativePos))
    {
      if (auto objectPtr = d_container.getItem(idx))
        notify(InventoryItemClickedEvent(objectPtr));

      if (d_selectionEnabled)
        selectItem(idx);

      return true;
    }
  }

  return false;
}
