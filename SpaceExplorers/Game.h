#pragma once

#include "Gui.h"
#include "IApp.h"
#include "World.h"

#include <LaggyDx/LaggyDxFwd.h>

#include <memory>


class Game
{
public:
  Game(IApp& i_app, const Dx::IResourceController& i_resourceController);

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

  void handleKeyboard(const Dx::KeyboardState& i_keyboardState);
  void handleMouse(const Dx::MouseState& i_mouseState);

private:
  IApp& d_app;

  Gui d_gui;
  std::unique_ptr<World> d_world;
};
