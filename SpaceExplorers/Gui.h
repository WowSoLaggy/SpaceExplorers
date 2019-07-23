#pragma once

#include "Cursor.h"
#include "IGuiControl.h"

#include <LaggyDx/LaggyDxFwd.h>

#include <memory>


class Gui
{
public:
  Gui(const Dx::IResourceController& i_resourceController);

  Cursor& getCursor();
  const Cursor& getCursor() const;

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

private:
  const Dx::IResourceController& d_resourceController;

  Cursor d_cursor;

  std::vector<IGuiControlPtr> d_guiControls;
};
