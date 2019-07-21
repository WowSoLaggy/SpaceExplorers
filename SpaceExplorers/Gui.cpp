#include "stdafx.h"
#include "Gui.h"

#include <LaggyDx/IRenderer2d.h>


Gui::Gui(const Dx::IResourceController& i_resourceController)
  : d_resourceController(i_resourceController)
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
  d_cursor.update(i_dt);
}

void Gui::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.resetTranslation();
  d_cursor.render(i_renderer);
}
