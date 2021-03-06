#pragma once

#include "Cursor.h"
#include "Fwd.h"
#include "IGuiControl.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggySdk/EventHandler.h>
#include <LaggySdk/Vector.h>

#include <memory>
#include <unordered_map>
#include <vector>


class Gui : public Sdk::EventHandler
{
public:
  Gui(Game& i_game, Dx::IResourceController& i_resourceController, const Camera& i_camera);

  virtual void processEvent(const Sdk::IEvent& i_event) override;

  Cursor& getCursor();
  const Cursor& getCursor() const;

  void update(double i_dt);
  void render(Dx::IRenderer2d& i_renderer) const;

  void clearControls();
  void showLoadingScreen();
  void showMainMenu();
  void showIngameGui();

  std::shared_ptr<Label> createLabel(const std::string& i_name = "");
  std::shared_ptr<Inventory> createInventory(Container& io_container, int i_sizeX, int i_sizeY,
                                             const std::string& i_name = "");

  IGuiControlPtr getControl(const std::string& i_name) const;
  void deleteControl(IGuiControlPtr i_guiControl);
  void deleteControl(const std::string& i_name);

  bool onMouseClick(Dx::MouseKey i_button, const Sdk::Vector2I& i_mousePos);
  void onMouseRelease(Dx::MouseKey i_button, const Sdk::Vector2I& i_mousePos);
  void onMouseMove(const Sdk::Vector2I& i_mousePos);

private:
  Game& d_game;
  Dx::IResourceController& d_resourceController;
  Sdk::Vector2I d_clientSize;
  const Camera& d_camera;

  Cursor d_cursor;
  std::unordered_map<std::string, IGuiControlPtr> d_guiControls;
  std::vector<IGuiControlPtr> d_guiControlsVector;
  bool d_clearAll = false;
  std::vector<std::string> d_controlsToRemove;
  std::vector<IGuiControlPtr> d_controlsToAdd;
  void addRemoveControls();

  void addControl(IGuiControlPtr i_control, const std::string& i_name);

  std::shared_ptr<Panel> createPanel(const std::string& i_name = "");
  std::shared_ptr<Button> createButton(const std::string& i_name = "");
  std::shared_ptr<BuildProgressBar> createBuildProgressBar(const std::string& i_name = "");
};
