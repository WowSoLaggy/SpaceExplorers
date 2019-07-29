#pragma once

#include "Camera.h"
#include "Fwd.h"
#include "Gui.h"
#include "World.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggySdk/Vector.h>

#include <memory>


class Game
{
public:
  Game(IApp& i_app, const Dx::IResourceController& i_resourceController);

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

  void handleKeyboard(const Dx::KeyboardState& i_keyboardState);
  void handleMouse(const Dx::MouseState& i_mouseState);

  void showDebugLabel();
  void hideDebugLabel();
  void updateDebugLabel(std::string i_text) const;

private:
  IApp& d_app;

  Camera d_camera;
  Gui d_gui;
  std::unique_ptr<World> d_world;

  bool d_debugOutput = true;
  std::shared_ptr<Label> d_debugLabel;

  Sdk::Vector2I screenToWorld(Sdk::Vector2I i_coords) const;
  Sdk::Vector2I worldToTile(Sdk::Vector2I i_coords) const;
  Sdk::Vector2I screenToTile(Sdk::Vector2I i_coords) const;

  void onClick(Sdk::Vector2I i_mousePos);
  void onSelectInventory(int i_index);
};
