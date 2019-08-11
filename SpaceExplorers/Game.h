#pragma once

#include "Camera.h"
#include "Fwd.h"
#include "Gui.h"
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
  void updateDebugLabel(std::string i_text) const;

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

  Sdk::Vector2I cursorToTile() const;

  State d_state = State::NotLoaded;
  std::future<void> loadResourcesResult;
  void loadResources();

  void onMouseClick(Dx::MouseKey i_button);
  void onMouseRelease(Dx::MouseKey i_button);
  void onMouseMove();

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

  void onSelectInventory(int i_index);
  void onUnselectInventory();

  void tryInteract();

  //
  // BUILD MODE
  //

  const StructurePrototype* d_buildStructure = nullptr;
  Dx::Sprite d_buildSprite;

  bool isInBuildMode() const;

  void onEnterBuildMode(const StructurePrototype& i_buildStructure);
  void onExitBuildMode();
  
  void updateBuildMode();
  bool canBuild() const;

  void tryBuild();

  //
  // REMOVAL MODE
  //

  bool isInRemovalMode() const;
  bool d_isRemovalMode = false;
  StructurePtr d_structToRemove;

  void onEnterRemovalMode();
  void onExitRemovalMode();

  void updateRemovalMode();
  StructurePtr getStructToRemove() const;

  void tryRemove();

  //
  // CONTROL
  //

  bool d_controlAvatar = false;
  std::shared_ptr<Object> d_avatar;

  bool isControlAvatar() const { return d_controlAvatar; }

  void switchControlMode();
  void onControlAvatar();
  void onControlCamera();
  void lookAtAvatar();

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();
};
