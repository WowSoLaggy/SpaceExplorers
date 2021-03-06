#pragma once

#include "Camera.h"
#include "Fwd.h"
#include "Gui.h"
#include "OverlayOptions.h"
#include "Prototypes.h"
#include "World.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/Vector.h>

#include <future>
#include <memory>


class Game
{
public:
  Game(IApp& i_app, Dx::IResourceController& i_resourceController);

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

  void handleKeyboard(const Dx::KeyboardState& i_keyboardState);
  void handleMouse(const Dx::MouseState& i_mouseState);

  void showDebugLabel();
  void hideDebugLabel();
  void updateDebugLabel(int i_renderedSprites) const;

private:
  const std::string CursorTexture = "Cursor.png";
  const std::string CursorBuildTexture = "CursorBuild.png";
  const std::string CursorRemoveTexture = "CursorRemove.png";

  enum class State
  {
    NotLoaded,
    LoadingResources,
    Loaded,
  };

private:
  IApp& d_app;
  Dx::IResourceController& d_resourceController;

  Camera d_camera;
  Gui d_gui;
  std::unique_ptr<World> d_world;

  bool d_debugOutput = false;
  std::shared_ptr<Label> d_debugLabel;

  OverlayOption d_overlayOption = OverlayOption::None;

  Sdk::Vector2I cursorToTile() const;
  Sdk::Vector2I cursorToWorld() const;
  ObjectPtr getSelectedTool() const;

  State d_state = State::NotLoaded;
  std::future<bool> loadResourcesResult;
  bool loadResources();

  void onMouseClick(Dx::MouseKey i_button);
  void onMouseRelease(Dx::MouseKey i_button);
  void onMouseMove();
  void onMouseWheelChange(int i_wheelChange);

  //
  // STATES
  //

public:
  void onNewGame();
  void onLoadGame();
  void onExitGame();

private:

  void onGameStarted();
  void checkIsGameLoaded();
  void onGameLoaded();
  void onEnterGame();

  //
  // SAVE - LOAD GAME
  //

  void onSaveGame();
  void loadWorld();

  //
  // IN-GAME
  //

  void onRightClick();

  void onSelectInventory(int i_index);
  void onUnselectInventory();
  void onScrollInventory(int i_change);

  void tryInteract();
  void tryDrop();

  //
  // CONTROL
  //

  bool d_controlAvatar = false;
  std::shared_ptr<Avatar> d_avatar;

  bool isControlAvatar() const { return d_controlAvatar; }

  void switchControlMode();
  void onControlAvatar();
  void onControlCamera();
  void lookAtAvatar();
  void setAvatarInventory();
  void setFreeModeInventory();

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();

  void setWalk();
  void setRun();

  //
  // OVERLAYS
  //

  void resetOverlay();
  void onOverlayAtmosphere();
};
