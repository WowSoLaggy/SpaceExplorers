#include "stdafx.h"
#include "Game.h"

#include "Inventory.h"

#include <LaggyDx/ImageDescription.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>
#include <LaggySdk/Contracts.h>


namespace
{
  Sdk::Vector4F getBuildColor(bool i_canBuild)
  {
    return i_canBuild ? Sdk::Vector4F{ 0.8f, 1.5f, 0.8f, 0.7f } : Sdk::Vector4F{ 1.5f, 0.7f, 0.7f, 0.7f };
  }
} // anonymous NS


bool Game::isInBuildMode() const
{
  return d_buildStructure;
}


void Game::onSelectInventory(int i_index)
{
  auto inventory = std::dynamic_pointer_cast<Inventory>(d_gui.getControl("Inventory"));
  CONTRACT_EXPECT(inventory);

  if (inventory->getSelectedIndex() == i_index)
    onUnselectInventory();
  else
  {
    inventory->selectItem(i_index);
    if (const auto* selectedItem = inventory->getSelectedItem())
      onEnterBuildMode(*selectedItem);
    else if (isInBuildMode())
      onExitBuildMode();
  }
}

void Game::onUnselectInventory()
{
  auto inventory = std::dynamic_pointer_cast<Inventory>(d_gui.getControl("Inventory"));
  CONTRACT_EXPECT(inventory);

  inventory->unselectItem();

  if (isInBuildMode())
    onExitBuildMode();
}


void Game::onEnterBuildMode(const StructurePrototype& i_buildStructure)
{
  d_buildStructure = &i_buildStructure;

  const auto& texture = d_resourceController.getTextureResource(d_buildStructure->textureFileName);
  d_buildSprite.setTexture(&texture);
  d_buildSprite.setSize(texture.getDescription().size());

  updateBuildSprite();
}

void Game::onExitBuildMode()
{
  d_buildStructure = nullptr;
}


void Game::updateBuildSprite()
{
  const auto pos = tileToScreen(screenToTile(d_gui.getCursor().getPosition()));
  d_buildSprite.setPosition(pos);

  d_buildSprite.setColor(getBuildColor(canBuild()));
}

bool Game::canBuild() const
{
  CONTRACT_EXPECT(isInBuildMode());
  CONTRACT_ASSERT(d_buildStructure);

  const auto layer = d_buildStructure->layer;
  const auto* tile = d_world->getTile(screenToTile(d_gui.getCursor().getPosition()));

  if (!tile)
    return layer == Layer::Panneling;

  if (tile->hasStructureOnLayer(layer))
    return false;
  if (!tile->hasStructureOnLayer(getPrevLayer(layer)))
    return false;

  return true;
}


void Game::tryBuild()
{
  CONTRACT_EXPECT(isInBuildMode());
  CONTRACT_ASSERT(d_world);
  CONTRACT_ASSERT(d_buildStructure);

  if (!canBuild())
    return;

  d_world->createStructureAt(*d_buildStructure, screenToTile(d_gui.getCursor().getPosition()));
}
