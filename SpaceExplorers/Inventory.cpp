#include "stdafx.h"
#include "Inventory.h"

#include "Prototypes.h"
#include "SettingsProvider.h"

#include <LaggyDx/ImageDescription.h>
#include <LaggyDx/ITextureResource.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/IRenderer2d.h>
#include <LaggySdk/Contracts.h>


Inventory::Inventory(Dx::IResourceController& i_resourceController)
  : d_resourceController(i_resourceController)
  , d_fontResource(i_resourceController.getFontResource(SettingsProvider::getDefaultInternalSettings().defaultFontName))
{
  d_items.resize(SlotsCount, nullptr);
  recreateSprites();
}


void Inventory::render(Dx::IRenderer2d& i_renderer) const
{
  const auto initTranslation = i_renderer.getTranslation();
  i_renderer.setTranslation(-d_position);

  // Grid
  for (const auto& sprite : d_gridSprites)
    i_renderer.renderSprite(sprite);

  // Items
  for (const auto& sprite : d_itemSprites)
    i_renderer.renderSprite(sprite);

  // Selection
  if (hasSelection())
    i_renderer.renderSprite(d_selectionSprite);

  // Quantity
  for (int i = 0; i < SlotsCount; ++i)
  {
    if (!d_items[i])
      continue;
    
    i_renderer.renderText(std::to_string(d_items[i]->getQuantity()), d_fontResource, d_itemSprites[i].getPosition());
  }


  i_renderer.setTranslation(initTranslation);
}


Sdk::Vector2I Inventory::getSize() const
{
  return { CornerSize * 2 + SlotsHor * SlotSize, CornerSize * 2 + SlotsVert * SlotSize };
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
    &textureTr, { CornerSize + SlotsHor * SlotSize, 0 },
    textureTr.getDescription().size(), Sdk::Vector4F::identity() });
  d_gridSprites.emplace_back(Dx::Sprite{
    &textureBl, { 0, CornerSize + SlotsVert * SlotSize },
    textureBl.getDescription().size(), Sdk::Vector4F::identity() });
  d_gridSprites.emplace_back(Dx::Sprite{
    &textureBr, { CornerSize + SlotsHor * SlotSize, CornerSize + SlotsVert * SlotSize },
    textureBr.getDescription().size(), Sdk::Vector4F::identity() });

  for (int i = 0; i < SlotsHor; ++i)
  {
    d_gridSprites.emplace_back(Dx::Sprite{
      &textureT, { CornerSize + SlotSize * i, 0 },
      textureT.getDescription().size(), Sdk::Vector4F::identity() });
    d_gridSprites.emplace_back(Dx::Sprite{
      &textureB, { CornerSize + SlotSize * i, CornerSize + SlotsVert * SlotSize },
      textureB.getDescription().size(), Sdk::Vector4F::identity() });
  }

  for (int i = 0; i < SlotsVert; ++i)
  {
    d_gridSprites.emplace_back(Dx::Sprite{
      &textureL, { 0, CornerSize + SlotSize * i },
      textureL.getDescription().size(), Sdk::Vector4F::identity() });
    d_gridSprites.emplace_back(Dx::Sprite{
      &textureR, { CornerSize + SlotSize * SlotsHor, CornerSize + SlotSize * i },
      textureR.getDescription().size(), Sdk::Vector4F::identity() });
  }

  for (int y = 0; y < SlotsVert; ++y)
  {
    for (int x = 0; x < SlotsHor; ++x)
    {
      d_gridSprites.emplace_back(Dx::Sprite{
        &textureItem, { CornerSize + SlotSize * x, CornerSize + SlotSize * y },
        textureItem.getDescription().size(), Sdk::Vector4F::identity() });

      d_itemSprites.emplace_back(Dx::Sprite{
        nullptr, { CornerSize + SlotSize * x + 4, CornerSize + SlotSize * y + 4 },
        textureItem.getDescription().size(), Sdk::Vector4F::identity() });

      updateItemSprite(x + y * SlotsHor);
    }
  }

  d_selectionSprite = {
    &textureFrame, { CornerSize + 2, CornerSize + 2 },
    textureFrame.getDescription().size(), Sdk::Vector4F::identity() };
}


void Inventory::CheckIndex(int i_index)
{
  CONTRACT_EXPECT(0 <= i_index);
  CONTRACT_EXPECT(i_index < SlotsCount);
}


void Inventory::resetAllItems()
{
  for (int i = 0; i < SlotsCount; ++i)
    resetItem(i);
}

void Inventory::resetItem(int i_index)
{
  CheckIndex(i_index);

  d_items.at(i_index) = nullptr;
  updateItemSprite(i_index);
}

void Inventory::setItem(int i_index, ObjectPtr i_object)
{
  CheckIndex(i_index);

  d_items.at(i_index) = i_object;
  updateItemSprite(i_index);
}

ObjectPtr Inventory::getItem(int i_index) const
{
  CheckIndex(i_index);

  return d_items.at(i_index);
}

void Inventory::updateItemSprite(int i_index)
{
  CheckIndex(i_index);

  auto item = d_items.at(i_index);
  const Dx::ITextureResource* texture = item ?
    &d_resourceController.getTextureResource(item->getPrototype().textureFileName) : nullptr;

  d_itemSprites.at(i_index).setTexture(texture);
}


void Inventory::selectItem(int i_index)
{
  CheckIndex(i_index);

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
  return d_items.at(*d_selectedIndex);
}

bool Inventory::hasSelection() const
{
  return d_selectedIndex.has_value();
}

void Inventory::updateSelectionSprite()
{
  if (!d_selectedIndex)
    return;

  int x = *d_selectedIndex % SlotsHor;
  int y = *d_selectedIndex / SlotsHor;
  d_selectionSprite.setPosition({ CornerSize + SlotSize * x + 2, CornerSize + SlotSize * y + 2 });
}


std::optional<int> Inventory::getFreeSlot() const
{
  for (int i = 0; i < SlotsCount; ++i)
  {
    if (!d_items[i])
      return i;
  }

  return std::nullopt;
}

std::optional<int> Inventory::getObjectIndex(ObjectPtr i_object) const
{
  for (int i = 0; i < SlotsCount; ++i)
  {
    if (d_items[i] && d_items[i]->canBeStackedWith(i_object))
      return i;
  }

  return std::nullopt;
}


bool Inventory::tryAddObject(ObjectPtr i_object)
{
  std::optional<int> index;
  if (i_object->getPrototype().isStackable)
  {
    if (index = getObjectIndex(i_object))
    {
      getItem(*index)->addQuantity(1);
      return true;
    }
  }

  index = getFreeSlot();
  if (!index)
  {
    // No free space!
    return false;
  }

  setItem(*index, i_object);
  return true;
}
