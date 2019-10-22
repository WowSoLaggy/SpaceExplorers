#include "stdafx.h"
#include "Game.h"

#include "Inventory.h"
#include "PrototypesCollection.h"

#include <LaggySdk/Contracts.h>


namespace
{
  const std::string InventoryControlName = "Inventory";
} // anonymous NS


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

  d_gui.connectTo(*d_avatar);
}

void Game::onControlCamera()
{
  d_gui.disconnectFrom(*d_avatar);

  onUnselectInventory();
  setFreeModeInventory();

  if (d_avatar)
    d_avatar.reset();

  d_controlAvatar = false;
}

void Game::lookAtAvatar()
{
  d_camera.setLookAt(d_avatar->getPosition());
}

void Game::setFreeModeInventory()
{
  if (const auto inventory = std::dynamic_pointer_cast<Inventory>(d_gui.getControl(InventoryControlName)))
  {
    if (d_avatar)
    {
      d_avatar->disconnectFrom(*inventory);
      inventory->disconnectFrom(*d_avatar);
    }
    d_gui.deleteControl(InventoryControlName);
  }

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

  d_gui.deleteControl(InventoryControlName);

  auto& container = d_avatar->getInventory();
  auto inventory = d_gui.createInventory(container, container.getSize(), 1, InventoryControlName);

  const auto inventorySize = inventory->getSize();
  auto clientSize = d_camera.getViewport().size();
  inventory->setPosition({ (clientSize.x - inventorySize.x) / 2, clientSize.y - inventorySize.y });

  d_avatar->connectTo(*inventory);
  inventory->connectTo(*d_avatar);
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


void Game::onRightClick()
{
  onUnselectInventory();
  if (d_avatar->isInspectingContainer())
    d_avatar->stopInspectingContainer();
}


void Game::onSelectInventory(int i_index)
{
  auto inventory = std::dynamic_pointer_cast<Inventory>(d_gui.getControl("Inventory"));
  if (!inventory)
    return;

  if (inventory->getSelectedIndex() == i_index)
    onUnselectInventory();
  else
  {
    if (isControlAvatar() && d_avatar->isBuilding())
      d_avatar->stopBuilding();

    inventory->selectItem(i_index);
  }
}

void Game::onUnselectInventory()
{
  auto inventory = std::dynamic_pointer_cast<Inventory>(d_gui.getControl("Inventory"));
  if (!inventory)
    return;

  inventory->unselectItem();

  if (isControlAvatar())
  {
    CONTRACT_ASSERT(d_avatar);

    if (d_avatar->isBuilding())
      d_avatar->stopBuilding();
  }
}

void Game::onScrollInventory(int i_change)
{
  auto inventory = std::dynamic_pointer_cast<Inventory>(d_gui.getControl("Inventory"));
  if (!inventory)
    return;

  const auto curIdx = inventory->getSelectedIndex();

  int newIdx = curIdx.has_value() ? *curIdx + i_change : i_change > 0 ? i_change - 1 : i_change;
  if (newIdx < 0)
    newIdx += inventory->getContainerSize();
  if (newIdx >= inventory->getContainerSize())
    newIdx -= inventory->getContainerSize();

  onSelectInventory(newIdx);
}


void Game::tryInteract()
{
  const auto worldCoords = cursorToWorld();

  if (isControlAvatar())
  {
    if (auto obj = d_world->getObjectAt(worldCoords))
    {
      auto selectedTool = getSelectedTool();
      Action action = selectedTool ? Action::None : Action::Pickup;
      d_avatar->interact(action, obj, selectedTool, worldCoords);

      return;
    }
  }

  auto structure = d_world->getStructureAt(worldCoords);
  if (isControlAvatar())
    d_avatar->interact(Action::Default, structure, getSelectedTool(), worldCoords);
  else if (structure)
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
