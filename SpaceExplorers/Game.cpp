#include "stdafx.h"
#include "Game.h"

#include "Label.h"
#include "SettingsProvider.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>


Game::Game(IApp& i_app, const Dx::IResourceController& i_resourceController)
  : d_app(i_app)
  , d_resourceController(i_resourceController)
  , d_camera(SettingsProvider::getDefaultExternalSettings().clientWidth,
    SettingsProvider::getDefaultExternalSettings().clientHeight)
  , d_gui(i_resourceController, d_camera.getViewport().size())
{
  d_world = World::createTestWorld(d_camera.getViewport().size(), i_resourceController);
  d_gui.getCursor().setTexture("Cursor.png");
  d_gui.createIngameGui();

  if (d_debugOutput)
    showDebugLabel();
}


void Game::update(double i_dt)
{
  if (d_world)
    d_world->update(i_dt);
  d_gui.update(i_dt);
  d_camera.update(i_dt);
}

void Game::render(Dx::IRenderer2d& i_renderer) const
{
  if (d_world)
    d_world->render(i_renderer, d_camera.getViewport());

  if (d_buildStructure)
    i_renderer.renderSprite(d_buildSprite);

  d_gui.render(i_renderer);

  if (d_debugOutput)
  {
    const std::string str = "Sprites: " + std::to_string(i_renderer.resetRenderedSprites()) + "\n";
    updateDebugLabel(str);
  }
}


void Game::showDebugLabel()
{
  d_debugLabel = d_gui.createDebugLabel();
  d_debugOutput = true;
}

void Game::hideDebugLabel()
{
  d_debugOutput = false;
  d_debugLabel.reset();
}

void Game::updateDebugLabel(std::string i_text) const
{
  if (d_debugLabel)
    d_debugLabel->setText(i_text);
}
