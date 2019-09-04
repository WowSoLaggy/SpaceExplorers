#include "stdafx.h"
#include "Gui.h"


bool Gui::onMouseClick(Dx::MouseKey i_button, const Sdk::Vector2I& i_mousePos)
{
  for (auto&[_, controlPtr] : d_guiControls)
  {
    if (controlPtr->onMouseClick(i_button, i_mousePos))
      return true;
  }

  return false;
}


void Gui::onMouseRelease(Dx::MouseKey i_button, const Sdk::Vector2I& i_mousePos)
{
  for (auto&[_, controlPtr] : d_guiControls)
    controlPtr->onMouseRelease(i_button, i_mousePos);
}


void Gui::onMouseMove(const Sdk::Vector2I& i_mousePos)
{
  for (auto&[_, controlPtr] : d_guiControls)
    controlPtr->onMouseMove(i_mousePos);
}
