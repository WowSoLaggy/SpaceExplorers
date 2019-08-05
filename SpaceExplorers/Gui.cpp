#include "stdafx.h"
#include "Gui.h"

#include <LaggyDx/IRenderer2d.h>


Gui::Gui(Game& i_game, Dx::IResourceController& i_resourceController, Sdk::Vector2I i_clientSize)
  : d_game(i_game)
  , d_resourceController(i_resourceController)
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

IGuiControlPtr Gui::getControl(const std::string& i_name)
{
  return d_guiControls.at(i_name);
}
