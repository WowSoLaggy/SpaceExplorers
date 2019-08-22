#include "stdafx.h"
#include "Game.h"

#include "Inventory.h"
#include "Utils.h"


Sdk::Vector2I Game::cursorToTile() const
{
  return screenToTile(d_gui.getCursor().getPosition(), d_camera);
}

Sdk::Vector2I Game::cursorToWorld() const
{
  return screenToWorld(d_gui.getCursor().getPosition(), d_camera);
}


ObjectPtr Game::getSelectedTool() const
{
  if (auto inventory = std::dynamic_pointer_cast<Inventory>(d_gui.getControl("Inventory")))
    return inventory->getSelectedItem();

  return nullptr;
}
