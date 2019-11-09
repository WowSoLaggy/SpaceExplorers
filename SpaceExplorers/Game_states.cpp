#include "stdafx.h"
#include "Game.h"

#include "IApp.h"

#include <LaggySdk/Contracts.h>


void Game::onGameStarted()
{
  d_gui.showLoadingScreen();
  d_gui.getCursor().setTexture(CursorTexture);

  if (d_debugOutput)
    showDebugLabel();

  loadResourcesResult = std::async(&Game::loadResources, this);

  d_state = State::LoadingResources;
}

void Game::checkIsGameLoaded()
{
  CONTRACT_ASSERT(d_state == State::LoadingResources);

  if (loadResourcesResult.wait_for(0ms) == std::future_status::timeout)
    return;

  const bool success = loadResourcesResult.get();
  CONTRACT_ENSURE(success);

  onGameLoaded();
}

void Game::onGameLoaded()
{
  d_state = State::Loaded;

  d_gui.clearControls();
  d_gui.showMainMenu();
  d_gui.getCursor().show();
}

void Game::onNewGame()
{
  d_world = World::createTestWorld(d_camera.getViewport().size(), d_resourceController);
  onEnterGame();
}

void Game::onLoadGame()
{
  loadWorld();
  onEnterGame();
}

void Game::onEnterGame()
{
  d_gui.clearControls();
  d_gui.showIngameGui();
  onControlAvatar();
}

void Game::onExitGame()
{
  d_app.stop();
}
