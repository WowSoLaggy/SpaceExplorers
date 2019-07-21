#include "stdafx.h"
#include "Game.h"

#include <LaggyDx/IRenderer2d.h>

Game::Game(IApp& i_app, const Dx::IResourceController& i_resourceController)
  : d_app(i_app)
  , d_camera(i_app.getClientWidth(), i_app.getClientHeight())
  , d_gui(i_resourceController)
{
  // TODO: ae Remove it from here
  d_gui.getCursor().setTexture("Cursor.dds");
}


void Game::update(double i_dt)
{
  if (d_world)
    d_world->update(i_dt);
  d_gui.update(i_dt);
}

void Game::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.setTranslation(d_camera.getTopLeftTranslation());

  if (d_world)
    d_world->render(i_renderer);
  d_gui.render(i_renderer);
}
