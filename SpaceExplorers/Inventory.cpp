#include "stdafx.h"
#include "Inventory.h"

#include <LaggyDx/ImageDescription.h>
#include <LaggyDx/ITextureResource.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/IRenderer2d.h>


Inventory::Inventory(const Dx::IResourceController& i_resourceController)
  : d_resourceController(i_resourceController)
{
  recreateSprites();
}


void Inventory::update(double i_dt)
{
}

void Inventory::render(Dx::IRenderer2d& i_renderer) const
{
  const auto initTranslation = i_renderer.getTranslation();
  i_renderer.setTranslation(-d_position);

  for (const auto& sprite : d_sprites)
    i_renderer.renderSprite(sprite);

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

  d_sprites.emplace_back(Dx::Sprite{
    &textureTl, { 0, 0 },
    textureTl.getDescription().size(), Sdk::Vector4F::identity() });
  d_sprites.emplace_back(Dx::Sprite{
    &textureTr, { CornerSize + SlotsHor * SlotSize, 0 },
    textureTr.getDescription().size(), Sdk::Vector4F::identity() });
  d_sprites.emplace_back(Dx::Sprite{
    &textureBl, { 0, CornerSize + SlotsVert * SlotSize },
    textureBl.getDescription().size(), Sdk::Vector4F::identity() });
  d_sprites.emplace_back(Dx::Sprite{
    &textureBr, { CornerSize + SlotsHor * SlotSize, CornerSize + SlotsVert * SlotSize },
    textureBr.getDescription().size(), Sdk::Vector4F::identity() });

  for (int i = 0; i < SlotsHor; ++i)
  {
    d_sprites.emplace_back(Dx::Sprite{
      &textureT, { CornerSize + SlotSize * i, 0 },
      textureT.getDescription().size(), Sdk::Vector4F::identity() });
    d_sprites.emplace_back(Dx::Sprite{
      &textureB, { CornerSize + SlotSize * i, CornerSize + SlotsVert * SlotSize },
      textureB.getDescription().size(), Sdk::Vector4F::identity() });
  }

  for (int i = 0; i < SlotsVert; ++i)
  {
    d_sprites.emplace_back(Dx::Sprite{
      &textureL, { 0, CornerSize + SlotSize * i },
      textureL.getDescription().size(), Sdk::Vector4F::identity() });
    d_sprites.emplace_back(Dx::Sprite{
      &textureR, { CornerSize + SlotSize * SlotsHor, CornerSize + SlotSize * i },
      textureR.getDescription().size(), Sdk::Vector4F::identity() });
  }

  for (int y = 0; y < SlotsVert; ++y)
  {
    for (int x = 0; x < SlotsHor; ++x)
    {
      d_sprites.emplace_back(Dx::Sprite{
        &textureItem, { CornerSize + SlotSize * x, CornerSize + SlotSize * y },
        textureItem.getDescription().size(), Sdk::Vector4F::identity() });
    }
  }
}
