#include "stdafx.h"
#include "Game.h"

#include "IApp.h"

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

  if (i_mouseState.getLeftButtonState() == Dx::ButtonState::Pressed)
    onLClick();
  else if (i_mouseState.getRightButtonState() == Dx::ButtonState::Pressed)
    onRClick();

  if (isInBuildMode())
    updateBuildSprite();
}


void Game::onLClick()
{
  if (!d_world)
    return;

  const auto& mousePos = d_gui.getCursor().getPosition();
  const auto tileCoords = screenToTile(mousePos);

  auto* tile = d_world->getTile(tileCoords);
  if (!tile)
    return;

  auto structure = tile->getTopStructure();
  if (!structure)
    return;

  structure->interact();
}

void Game::onRClick()
{
  if (!d_world)
    return;

  onUnselectInventory();
}
