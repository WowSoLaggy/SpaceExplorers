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
}

void Game::handleMouse(const Dx::MouseState& i_mouseState)
{
  if (i_mouseState.getMode() == Dx::MouseMode::Absolute)
    d_gui.getCursor().setPosition(i_mouseState.getPosition());
  else
    d_gui.getCursor().movePosition(i_mouseState.getPosition());
}
