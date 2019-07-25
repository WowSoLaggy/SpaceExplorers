#include "stdafx.h"
#include "Game.h"

#include "Label.h"
#include "SettingsProvider.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>


Game::Game(IApp& i_app, const Dx::IResourceController& i_resourceController)
  : d_app(i_app)
  , d_camera(SettingsProvider::getDefaultExternalSettings().clientWidth,
    SettingsProvider::getDefaultExternalSettings().clientHeight)
  , d_gui(i_resourceController)
{
  d_gui.getCursor().setTexture("Cursor.png");
  d_world = World::createTestWorld(i_resourceController);

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
  i_renderer.setTranslation(d_camera.getTopLeftPoint());

  int renderedObjects = 0;
  if (d_world)
    renderedObjects += d_world->render(i_renderer, d_camera.getViewport());
  d_gui.render(i_renderer);

  if (d_debugOutput)
  {
    std::string str = "Structures: " + std::to_string(renderedObjects) + "\n";
    str += "Mouse: ";
    str += d_mouseDown ? "DOWN" : "UP";
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
