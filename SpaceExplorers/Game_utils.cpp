#include "stdafx.h"
#include "Game.h"

#include "Utils.h"


Sdk::Vector2I Game::cursorToTile() const
{
  return screenToTile(d_gui.getCursor().getPosition(), d_camera);
}

Sdk::Vector2I Game::cursorToWorld() const
{
  return screenToWorld(d_gui.getCursor().getPosition(), d_camera);
}
