#include "stdafx.h"
#include "Game.h"

#include "IApp.h"
#include "Label.h"
#include "PrototypesCollection.h"
#include "SettingsProvider.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>
#include <LaggySdk/Contracts.h>


Game::Game(IApp& i_app, Dx::IResourceController& i_resourceController)
  : d_app(i_app)
  , d_resourceController(i_resourceController)
  , d_camera(SettingsProvider::getDefaultExternalSettings().clientWidth,
    SettingsProvider::getDefaultExternalSettings().clientHeight)
  , d_gui(*this, i_resourceController, d_camera.getViewport().size())
{
}


void Game::update(double i_dt)
{
  switch (d_state)
  {
  case Game::State::NotLoaded:
    onGameStarted();
    break;
  case Game::State::LoadingResources:
    checkIsGameLoaded();
    break;
  case Game::State::Loaded:
    [[fallthrough]];
  default:
    break;
  }

  if (d_world)
    d_world->update(i_dt);
  d_gui.update(i_dt);

  if (isControlAvatar())
    lookAtAvatar();

  d_camera.update(i_dt);
}

void Game::render(Dx::IRenderer2d& i_renderer) const
{
  if (d_world)
    d_world->render(i_renderer, d_camera.getViewport());

  d_gui.render(i_renderer);

  if (d_debugOutput)
  {
    const std::string str = "Sprites: " + std::to_string(i_renderer.getRenderedSprites()) + "\n";
    updateDebugLabel(str);
  }
}


void Game::showDebugLabel()
{
  d_debugLabel = d_gui.createLabel("DebugLabel");
  d_debugOutput = true;
}

void Game::hideDebugLabel()
{
  d_debugOutput = false;
  d_gui.deleteControl(d_debugLabel);
  d_debugLabel.reset();
}

void Game::updateDebugLabel(std::string i_text) const
{
  if (d_debugLabel)
    d_debugLabel->setText(i_text);
}


void Game::loadResources()
{
  PrototypesCollection::load();
  d_resourceController.loadResources();
  std::this_thread::sleep_for(100ms);
}
