#pragma once

#include "Cursor.h"
#include "Fwd.h"
#include "IGuiControl.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggySdk/Vector.h>

#include <memory>
#include <unordered_map>


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
  IGuiControlPtr getControl(const std::string& i_name);

private:
  const Dx::IResourceController& d_resourceController;
  Sdk::Vector2I d_clientSize;

  Cursor d_cursor;
  std::unordered_map<std::string, IGuiControlPtr> d_guiControls;

  std::shared_ptr<Inventory> createInventory();
};
