#include "stdafx.h"
#include "Game.h"

#include "IApp.h"
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
  , d_gui(*this, i_resourceController, d_camera)
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
    d_world->render(i_renderer, d_camera.getViewport(), d_overlayOption);

  d_gui.render(i_renderer);

  if (d_debugOutput)
    updateDebugLabel(i_renderer.getRenderedSprites());
}


bool Game::loadResources()
{
  try
  {
    PrototypesCollection::load();
    d_resourceController.loadResources();
    std::this_thread::sleep_for(100ms);
  }
  catch (...)
  {
    return false;
  }

  return true;
}


void Game::resetOverlay()
{
  d_overlayOption = OverlayOption::None;
}

void Game::onOverlayAtmosphere()
{
  if (d_overlayOption != OverlayOption::Atmosphere)
    d_overlayOption = OverlayOption::Atmosphere;
  else
    resetOverlay();
}
