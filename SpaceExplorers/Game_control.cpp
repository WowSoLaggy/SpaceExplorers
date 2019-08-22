#include "stdafx.h"
#include "Game.h"

#include "Inventory.h"
#include "PrototypesCollection.h"

#include <LaggySdk/Contracts.h>


void Game::switchControlMode()
{
  CONTRACT_EXPECT(d_world);

  if (isControlAvatar())
    onControlCamera();
  else
    onControlAvatar();
}

void Game::onControlAvatar()
{
  d_controlAvatar = true;

  d_avatar = d_world->getAvatarPtr("Tom");
  CONTRACT_ASSERT(d_avatar);

  lookAtAvatar();
  setAvatarInventory();
}

void Game::onControlCamera()
{
  d_controlAvatar = false;

  if (d_avatar)
    d_avatar.reset();

  onUnselectInventory();
  setFreeModeInventory();
}

void Game::lookAtAvatar()
{
  d_camera.setLookAt(d_avatar->getPosition());
}

void Game::setFreeModeInventory()
{
  d_gui.deleteControl("Inventory");

  // TODO: ae Free build mode objects
  // e.g. infinite of all types
  /*inventory->setItem(0, PrototypesCollection::getStructure("Lattice"));
  inventory->setItem(1, PrototypesCollection::getStructure("Floor"));
  inventory->setItem(2, PrototypesCollection::getStructure("Wall"));
  inventory->setItem(3, PrototypesCollection::getStructure("Door"));*/
}

void Game::setAvatarInventory()
{
  CONTRACT_EXPECT(d_avatar);

  d_gui.deleteControl("Inventory");

  auto& container = d_avatar->getInventory();
  auto inventory = d_gui.createInventory(container, container.getSize(), 1, "Inventory");

  const auto inventorySize = inventory->getSize();
  auto clientSize = d_camera.getViewport().size();
  inventory->setPosition({ (clientSize.x - inventorySize.x) / 2, clientSize.y - inventorySize.y });
}


void Game::moveLeft()
{
  if (d_controlAvatar)
    d_avatar->moveLeft();
  else
    d_camera.moveLeft();
}

void Game::moveRight()
{
  if (d_controlAvatar)
    d_avatar->moveRight();
  else
    d_camera.moveRight();
}

void Game::moveUp()
{
  if (d_controlAvatar)
    d_avatar->moveUp();
  else
    d_camera.moveUp();
}

void Game::moveDown()
{
  if (d_controlAvatar)
    d_avatar->moveDown();
  else
    d_camera.moveDown();
}


void Game::onSelectInventory(int i_index)
{
  auto inventory = std::dynamic_pointer_cast<Inventory>(d_gui.getControl("Inventory"));
  if (!inventory)
    return;

  if (inventory->getSelectedIndex() == i_index)
    onUnselectInventory();
  else
    inventory->selectItem(i_index);
}

void Game::onUnselectInventory()
{
  auto inventory = std::dynamic_pointer_cast<Inventory>(d_gui.getControl("Inventory"));
  if (!inventory)
    return;

  inventory->unselectItem();
}


void Game::tryInteract()
{
  if (d_avatar)
  {
    if (auto obj = d_world->getObjectAt(cursorToWorld()))
    {
      auto selectedTool = getSelectedTool();
      Action action = selectedTool ? Action::None : Action::Pickup;
      d_avatar->interact(action, obj, selectedTool);
      return;
    }
  }

  const auto tileCoords = cursorToTile();

  auto* tile = d_world->getTile(tileCoords);
  if (!tile)
    return;

  auto structure = tile->getTopStructure();
  if (!structure)
    return;

  if (d_avatar)
    d_avatar->interact(Action::Default, structure, getSelectedTool());
  else
    structure->interact();
}

void Game::tryDrop()
{
  if (!d_avatar)
    return;

  auto selectedItem = getSelectedTool();
  if (!selectedItem)
    return;

  d_avatar->interact(Action::Drop, nullptr, selectedItem, cursorToWorld());
}
