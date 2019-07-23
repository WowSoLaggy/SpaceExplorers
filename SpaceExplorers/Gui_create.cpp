#include "stdafx.h"
#include "Gui.h"

#include "Label.h"


std::shared_ptr<Label> Gui::createDebugLabel()
{
  auto label = std::make_shared<Label>(d_resourceController);
  d_guiControls.push_back(label);
  return label;
}
