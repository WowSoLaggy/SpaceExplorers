#include "stdafx.h"
#include "Game.h"

#include "GInventory.h"
#include "PrototypesCollection.h"

#include <LaggySdk/Contracts.h>


namespace
{
  void setFreeModeInventory(Gui& io_gui)
  {
    auto inventory = std::dynamic_pointer_cast<GInventory>(io_gui.getControl("Inventory"));
    CONTRACT_ASSERT(inventory);

    inventory->resetAllItems();

    inventory->setItem(0, PrototypesCollection::getStructure("Lattice"));
    inventory->setItem(1, PrototypesCollection::getStructure("Floor"));
    inventory->setItem(2, PrototypesCollection::getStructure("Wall"));
    inventory->setItem(3, PrototypesCollection::getStructure("Door"));
  }

  void setAvatarInventory(Gui& io_gui, const Object& i_object)
  {
    auto inventory = std::dynamic_pointer_cast<GInventory>(io_gui.getControl("Inventory"));
    CONTRACT_ASSERT(inventory);

    inventory->resetAllItems();
  }

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
  setAvatarInventory(d_gui, *d_avatar);
}

void Game::onControlCamera()
{
  d_controlAvatar = false;

  if (d_avatar)
    d_avatar.reset();

  onUnselectInventory();
  if (isInRemovalMode())
    onExitRemovalMode();
  setFreeModeInventory(d_gui);
}

void Game::lookAtAvatar()
{
  d_camera.setLookAt(d_avatar->getPosition());
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
