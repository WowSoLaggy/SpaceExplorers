#pragma once

#include "Cursor.h"
#include "Fwd.h"
#include "IGuiControl.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggySdk/Vector.h>

#include <memory>


class Gui
{
public:
  Gui(const Dx::IResourceController& i_resourceController, Sdk::Vector2I i_clientSize);

  Cursor& getCursor();
  const Cursor& getCursor() const;

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

  void createIngameGui();

  std::shared_ptr<Label> createDebugLabel();

private:
  const Dx::IResourceController& d_resourceController;
  Sdk::Vector2I d_clientSize;

  Cursor d_cursor;
  std::vector<IGuiControlPtr> d_guiControls;

  std::shared_ptr<Inventory> createInventory();
};
