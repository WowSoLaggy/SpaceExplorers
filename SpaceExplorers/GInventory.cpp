#include "stdafx.h"
#include "GInventory.h"

#include "Prototypes.h"

#include <LaggyDx/ImageDescription.h>
#include <LaggyDx/ITextureResource.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/IRenderer2d.h>
#include <LaggySdk/Contracts.h>


GInventory::GInventory(Dx::IResourceController& i_resourceController)
  : d_resourceController(i_resourceController)
{
  d_items.resize(SlotsCount, nullptr);
  recreateSprites();
}


void GInventory::render(Dx::IRenderer2d& i_renderer) const
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

  i_renderer.setTranslation(initTranslation);
}


Sdk::Vector2I GInventory::getSize() const
{
  return { CornerSize * 2 + SlotsHor * SlotSize, CornerSize * 2 + SlotsVert * SlotSize };
}


void GInventory::recreateSprites()
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


void GInventory::CheckIndex(int i_index)
{
  CONTRACT_EXPECT(0 <= i_index);
  CONTRACT_EXPECT(i_index < SlotsCount);
}


void GInventory::resetAllItems()
{
  for (int i = 0; i < SlotsCount; ++i)
    resetItem(i);
}

void GInventory::resetItem(int i_index)
{
  CheckIndex(i_index);

  d_items.at(i_index) = nullptr;
  updateItemSprite(i_index);
}

void GInventory::setItem(int i_index, const StructurePrototype& i_prototype)
{
  CheckIndex(i_index);

  d_items.at(i_index) = &i_prototype;
  updateItemSprite(i_index);
}

const StructurePrototype* GInventory::getItem(int i_index) const
{
  CheckIndex(i_index);

  return d_items.at(i_index);
}

void GInventory::updateItemSprite(int i_index)
{
  CheckIndex(i_index);

  const auto* item = d_items.at(i_index);
  const Dx::ITextureResource* texture = item ?
    &d_resourceController.getTextureResource(item->textureFileName) : nullptr;

  d_itemSprites.at(i_index).setTexture(texture);
}


void GInventory::selectItem(int i_index)
{
  CheckIndex(i_index);

  d_selectedIndex = i_index;
  updateSelectionSprite();
}

void GInventory::unselectItem()
{
  d_selectedIndex = std::nullopt;
}

std::optional<int> GInventory::getSelectedIndex() const
{
  return d_selectedIndex;
}

const StructurePrototype* GInventory::getSelectedItem() const
{
  if (!d_selectedIndex)
    return nullptr;
  return d_items.at(*d_selectedIndex);
}

bool GInventory::hasSelection() const
{
  return d_selectedIndex.has_value();
}

void GInventory::updateSelectionSprite()
{
  if (!d_selectedIndex)
    return;

  int x = *d_selectedIndex % SlotsHor;
  int y = *d_selectedIndex / SlotsHor;
  d_selectionSprite.setPosition({ CornerSize + SlotSize * x + 2, CornerSize + SlotSize * y + 2 });
}
