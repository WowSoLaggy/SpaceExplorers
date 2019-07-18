#include "stdafx.h"
#include "Game.h"

#include <LaggyDx/KeyboardState.h>
#include <LaggyDx/MouseState.h>


void Game::handleKeyboard(const Dx::KeyboardState& i_keyboardState)
{
  if (i_keyboardState.getPressedKeys().Escape)
    d_app.stop();
}

void Game::handleMouse(const Dx::MouseState& i_mouseState)
{
  d_gui.getCursor().setPosition(i_mouseState.getPosition());
}
