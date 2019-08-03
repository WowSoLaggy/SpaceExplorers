#include "stdafx.h"
#include "Game.h"

#include "Inventory.h"

#include <LaggyDx/ImageDescription.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>
#include <LaggySdk/Contracts.h>


namespace
{
  const Sdk::Vector4F ColorCanBuild = { 0.8f, 1.5f, 0.8f, 0.7f };
  const Sdk::Vector4F ColorCannotBuild = { 1.5f, 0.7f, 0.7f, 0.7f };
  const Sdk::Vector4F ColorRemoval = { 1.5f, 0.7f, 0.7f, 1.0f };
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
  if (isInRemovalMode())
    onExitRemovalMode();

  d_buildStructure = &i_buildStructure;

  const auto& texture = d_resourceController.getTextureResource(d_buildStructure->textureFileName);
  d_buildSprite.setTexture(&texture);
  d_buildSprite.setSize(texture.getDescription().size());

  updateBuildMode();
}

void Game::onExitBuildMode()
{
  d_buildStructure = nullptr;
}


void Game::updateBuildMode()
{
  const auto pos = tileToScreen(cursorToTile());
  d_buildSprite.setPosition(pos);

  d_buildSprite.setColor(canBuild() ? ColorCanBuild : ColorCannotBuild);
}

bool Game::canBuild() const
{
  CONTRACT_EXPECT(isInBuildMode());
  CONTRACT_ASSERT(d_buildStructure);

  const auto layer = d_buildStructure->layer;
  const auto* tile = d_world->getTile(cursorToTile());

  if (!tile || !tile->hasStructures())
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

  d_world->createStructureAt(*d_buildStructure, cursorToTile());
}


//
// REMOVAL MODE
//


bool Game::isInRemovalMode() const
{
  return d_isRemovalMode;
}


void Game::onEnterRemovalMode()
{
  if (isInBuildMode())
    onExitBuildMode();

  d_isRemovalMode = true;
  updateRemovalMode();
}

void Game::onExitRemovalMode()
{
  if (d_structToRemove)
  {
    d_structToRemove->resetColor();
    d_structToRemove = nullptr;
  }

  d_isRemovalMode = false;
}


void Game::updateRemovalMode()
{
  CONTRACT_ASSERT(isInRemovalMode());
  
  auto structToRemove = getStructToRemove();
  if (structToRemove == d_structToRemove)
    return;

  if (d_structToRemove)
    d_structToRemove->resetColor();

  d_structToRemove = structToRemove;
  if (!d_structToRemove)
    return;

  d_structToRemove->setColor(ColorRemoval);
}

StructurePtr Game::getStructToRemove() const
{
  const auto* tile = d_world->getTile(cursorToTile());
  if (!tile)
    return nullptr;

  return tile->getTopStructure();
}


void Game::tryRemove()
{
  if (!d_structToRemove)
    return;

  auto* tile = d_world->getTile(d_structToRemove->getCoordsTile());
  CONTRACT_EXPECT(tile);

  tile->removeStructure(d_structToRemove);
  updateRemovalMode();
}
