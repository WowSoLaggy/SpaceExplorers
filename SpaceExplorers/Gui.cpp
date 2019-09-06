#include "stdafx.h"
#include "Gui.h"

#include "Avatar_events.h"
#include "BuildProgressBar.h"
#include "Camera.h"
#include "SettingsProvider.h"
#include "Utils.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggySdk/Contracts.h>


Gui::Gui(Game& i_game, Dx::IResourceController& i_resourceController, const Camera& i_camera)
  : d_game(i_game)
  , d_resourceController(i_resourceController)
  , d_clientSize(i_camera.getViewport().size())
  , d_cursor(i_resourceController)
  , d_camera(i_camera)
{
}


void Gui::processEvent(const Sdk::IEvent& i_event)
{
  static const std::string AvatarBuildProgressBarName = "BuildProgressBar";

  auto getBuildProgressBarPosition = [&](const Sdk::Vector2I& i_tileCoords, const Sdk::Vector2I& i_barSize) {
    static const int tileSize = SettingsProvider::getDefaultInternalSettings().tileSize;
    auto pos = worldToTile(i_tileCoords) * tileSize;
    pos.x += (tileSize - i_barSize.x) / 2;
    pos.y -= i_barSize.y;
    return worldToScreen(pos, d_camera);
  };


  if (const auto* event = dynamic_cast<const StartBuildEvent*>(&i_event))
  {
    auto bar = createBuildProgressBar(AvatarBuildProgressBarName);
    bar->setProgress(event->getProgress());
    bar->setObjectPosition(getBuildProgressBarPosition(event->getPosition(), bar->getSize()));
  }


  else if (const auto* event = dynamic_cast<const UpdateBuildEvent*>(&i_event))
  {
    auto bar = std::dynamic_pointer_cast<BuildProgressBar>(getControl(AvatarBuildProgressBarName));
    CONTRACT_ASSERT(bar);
    bar->setProgress(event->getProgress());
    bar->setObjectPosition(getBuildProgressBarPosition(event->getPosition(), bar->getSize()));
  }


  else if (dynamic_cast<const StopBuildEvent*>(&i_event))
  {
    deleteControl(AvatarBuildProgressBarName);
  }
}


Cursor& Gui::getCursor()
{
  return d_cursor;
}

const Cursor& Gui::getCursor() const
{
  return d_cursor;
}


void Gui::addRemoveControls()
{
  if (d_clearAll)
  {
    d_guiControls.clear();
    d_controlsToRemove.clear();
    d_guiControlsVector.clear();
    d_clearAll = false;
  }
  else if (!d_controlsToRemove.empty())
  {
    for (auto& name : d_controlsToRemove)
      d_guiControls.erase(name);

    // Remove all controls with given names
    d_guiControlsVector.erase(std::remove_if(d_guiControlsVector.begin(), d_guiControlsVector.end(),
                              [&](auto controlPtr) {
      return std::any_of(d_controlsToRemove.begin(), d_controlsToRemove.end(), [&](auto i_name) {
        return controlPtr->getName() == i_name;
      });
    }), d_guiControlsVector.end());

    d_controlsToRemove.clear();
  }

  if (!d_controlsToAdd.empty())
  {
    for (auto& control : d_controlsToAdd)
    {
      d_guiControls[control->getName()] = control;
      d_guiControlsVector.push_back(control);
    }
    d_controlsToAdd.clear();
  }
}


void Gui::update(double i_dt)
{
  addRemoveControls();

  for (const auto& control : d_guiControlsVector)
    control->update(i_dt);

  d_cursor.update(i_dt);
}

void Gui::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.resetTranslation();

  for (const auto control : d_guiControlsVector)
    control->render(i_renderer);

  d_cursor.render(i_renderer);
}


void Gui::clearControls()
{
  d_clearAll = true;
}

IGuiControlPtr Gui::getControl(const std::string& i_name) const
{
  if (auto it = d_guiControls.find(i_name); it != d_guiControls.end())
    return it->second;

  const auto it = std::find_if(d_controlsToAdd.cbegin(), d_controlsToAdd.cend(), [&](const IGuiControlPtr& i_control) {
    return i_control->getName() == i_name;
  });

  return it != d_controlsToAdd.cend() ? *it : nullptr;
}
