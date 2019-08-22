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

  if (pressedKeys.OemTilde)
  {
    if (d_debugOutput)
      hideDebugLabel();
    else
      showDebugLabel();
  }

  if (!d_world)
    return;


  if (pressedKeys.C)
    switchControlMode();

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

  if (pressedKeys.Add)
    onSaveGame();

  if (currentKeys.A)
    moveLeft();
  if (currentKeys.D)
    moveRight();
  if (currentKeys.W)
    moveUp();
  if (currentKeys.S)
    moveDown();

  if (!isControlAvatar())
    return;

  if (pressedKeys.Q)
    tryDrop();
}

void Game::handleMouse(const Dx::MouseState& i_mouseState)
{
  const auto& mousePosRaw = i_mouseState.getPosition();
  if (i_mouseState.getMode() == Dx::MouseMode::Absolute)
  {
    if (mousePosRaw != d_gui.getCursor().getPosition())
    {
      d_gui.getCursor().setPosition(mousePosRaw);
      onMouseMove();
    }
  }
  else
  {
    if (mousePosRaw != Sdk::Vector2I{ 0, 0 })
    {
      d_gui.getCursor().movePosition(mousePosRaw);
      onMouseMove();
    }
  }

  if (i_mouseState.getLeftButtonState() == Dx::ButtonState::Pressed)
    onMouseClick(Dx::MouseKey::Left);
  else if (i_mouseState.getLeftButtonState() == Dx::ButtonState::Released)
    onMouseRelease(Dx::MouseKey::Left);
  
  if (i_mouseState.getRightButtonState() == Dx::ButtonState::Pressed)
    onMouseClick(Dx::MouseKey::Right);
  else if (i_mouseState.getRightButtonState() == Dx::ButtonState::Released)
    onMouseRelease(Dx::MouseKey::Right);
}


void Game::onMouseClick(Dx::MouseKey i_button)
{
  d_gui.onMouseClick(i_button, d_gui.getCursor().getPosition());

  if (!d_world)
    return;

  if (i_button == Dx::MouseKey::Left)
    tryInteract();
  else if (i_button == Dx::MouseKey::Right)
    onUnselectInventory();
}

void Game::onMouseRelease(Dx::MouseKey i_button)
{
  d_gui.onMouseRelease(i_button, d_gui.getCursor().getPosition());
}

void Game::onMouseMove()
{
  d_gui.onMouseMove(d_gui.getCursor().getPosition());
}
