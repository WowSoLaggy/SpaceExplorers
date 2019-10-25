#include "stdafx.h"
#include "Game.h"

#include "IApp.h"
#include "Label.h"
#include "PrototypesCollection.h"
#include "SettingsProvider.h"
#include "Utils.h"

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

void Game::updateDebugLabel(int i_renderedSprites) const
{
  if (!d_debugLabel)
    return;

  std::string str = "Sprites: " + std::to_string(i_renderedSprites) + "\n";

  str += "Cursor coords: " + std::to_string(d_gui.getCursor().getPosition().x) + "x, " +
    std::to_string(d_gui.getCursor().getPosition().y) + "y\n";

  if (d_world)
  {
    const auto tileCoords = screenToTile(d_gui.getCursor().getPosition(), d_camera);
    str += "Cursor tile coords: " + std::to_string(tileCoords.x) + "x, " +
      std::to_string(tileCoords.y) + "y\n";

    if (const auto* tile = d_world->getTile(tileCoords))
    {
      const auto& atmo = tile->getAtmosphere();
      const auto ratios = atmo.getGasesRatios();

      str += "========================\n";
      str += "Pressure: " + std::to_string(atmo.getPressure()) + " Pa\n";
      for (const auto&[type, amount] : atmo.getGases())
      {
        const int ratio = (int)(ratios.at(type) * 100);
        str += "--- " + GasNames.at(type) + ": " + std::to_string(amount) +
          " (" + std::to_string(ratio) + " %)\n";
      }
    }
  }

  d_debugLabel->setText(str);
}


void Game::loadResources()
{
  PrototypesCollection::load();
  d_resourceController.loadResources();
  std::this_thread::sleep_for(100ms);
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
