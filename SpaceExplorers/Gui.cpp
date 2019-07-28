#include "stdafx.h"
#include "Gui.h"

#include <LaggyDx/IRenderer2d.h>


Gui::Gui(const Dx::IResourceController& i_resourceController, Sdk::Vector2I i_clientSize)
  : d_resourceController(i_resourceController)
  , d_clientSize(i_clientSize)
  , d_cursor(i_resourceController)
{
}


Cursor& Gui::getCursor()
{
  return d_cursor;
}

const Cursor& Gui::getCursor() const
{
  return d_cursor;
}


void Gui::update(double i_dt)
{
  for (const auto gui : d_guiControls)
    gui->update(i_dt);

  d_cursor.update(i_dt);
}

void Gui::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.resetTranslation();

  for (const auto gui : d_guiControls)
    gui->render(i_renderer);

  d_cursor.render(i_renderer);
}
