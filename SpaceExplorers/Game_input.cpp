#include "stdafx.h"
#include "Game.h"

#include "IApp.h"
#include "Inventory.h"

#include <LaggyDx/KeyboardState.h>
#include <LaggyDx/MouseState.h>


void Game::handleKeyboard(const Dx::KeyboardState& i_keyboardState)
{
  const auto& pressedKeys = i_keyboardState.getPressedKeys();
  const auto& currentKeys = i_keyboardState.getCurrentKeys();

  if (pressedKeys.Escape)
    d_app.stop();

  if (pressedKeys.F12)
  {
    if (d_debugOutput)
      hideDebugLabel();
    else
      showDebugLabel();
  }

  if (currentKeys.A)
    d_camera.moveLeft();
  if (currentKeys.D)
    d_camera.moveRight();
  if (currentKeys.W)
    d_camera.moveUp();
  if (currentKeys.S)
    d_camera.moveDown();

  if (pressedKeys.D1)
    onSelectInventory(0);
  else if (pressedKeys.D2)
    onSelectInventory(1);
  else if (pressedKeys.D3)
    onSelectInventory(2);
  else if (pressedKeys.D4)
    onSelectInventory(3);
  else if (pressedKeys.D5)
    onSelectInventory(4);
  else if (pressedKeys.D6)
    onSelectInventory(5);
  else if (pressedKeys.D7)
    onSelectInventory(6);
  else if (pressedKeys.D8)
    onSelectInventory(7);
}

void Game::handleMouse(const Dx::MouseState& i_mouseState)
{
  const auto& mousePosRaw = i_mouseState.getPosition();
  if (i_mouseState.getMode() == Dx::MouseMode::Absolute)
    d_gui.getCursor().setPosition(mousePosRaw);
  else
    d_gui.getCursor().movePosition(mousePosRaw);

  const auto& mousePos = d_gui.getCursor().getPosition();

  if (i_mouseState.getLeftButtonState() == Dx::ButtonState::Pressed)
    onClick(mousePos);
}


void Game::onClick(Sdk::Vector2I i_mousePos)
{
  if (!d_world)
    return;

  const auto tileCoords = screenToTile(i_mousePos);

  auto* tile = d_world->getTile(tileCoords);
  if (!tile)
    return;

  auto structure = tile->getTopStructure();
  if (!structure)
    return;

  structure->interact();
}


void Game::onSelectInventory(int i_index)
{
  auto inventory = std::dynamic_pointer_cast<Inventory>(d_gui.getControl("Inventory"));
  if (!inventory)
    return;

  if (inventory->getSelectedIndex() == i_index)
    inventory->unselectItem();
  else
    inventory->selectItem(i_index);
}
