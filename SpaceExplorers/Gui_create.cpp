#include "stdafx.h"
#include "Gui.h"

#include "Button.h"
#include "Inventory.h"
#include "Label.h"
#include "Panel.h"
#include "Prototypes.h"

#include <LaggySdk/StringUtils.h>


namespace
{
  std::string randStr()
  {
    return Sdk::generateRandomString(32);
  }
} // anonymous NS

void Gui::showIngameGui()
{
  auto inventory = createInventory("Inventory");
  const auto inventorySize = inventory->getSize();
  inventory->setPosition({ (d_clientSize.x - inventorySize.x) / 2, d_clientSize.y - inventorySize.y });

  inventory->setItem(0, Prototypes::Lattice());
  inventory->setItem(1, Prototypes::Floor());
  inventory->setItem(2, Prototypes::Wall());
  inventory->setItem(3, Prototypes::Door());
}


std::shared_ptr<Label> Gui::createDebugLabel(const std::string& i_name /*= ""*/)
{
  auto control = std::make_shared<Label>(d_resourceController);
  addControl(control, i_name.empty() ? randStr() : i_name);
  return control;
}

std::shared_ptr<Panel> Gui::createPanel(const std::string& i_name /*= ""*/)
{
  auto control = std::make_shared<Panel>(d_resourceController);
  addControl(control, i_name.empty() ? randStr() : i_name);
  return control;
}

std::shared_ptr<Inventory> Gui::createInventory(const std::string& i_name /*= ""*/)
{
  auto control = std::make_shared<Inventory>(d_resourceController);
  addControl(control, i_name.empty() ? randStr() : i_name);
  return control;
}

std::shared_ptr<Button> Gui::createButton(const std::string& i_name /*= ""*/)
{
  auto control = std::make_shared<Button>(d_resourceController);
  addControl(control, i_name.empty() ? randStr() : i_name);
  return control;
}


void Gui::addControl(IGuiControlPtr i_control, const std::string& i_name)
{
  d_guiControls[i_name] = i_control;
  i_control->setName(std::move(i_name));
}


void Gui::deleteControl(IGuiControlPtr i_guiControl)
{
  deleteControl(i_guiControl->getName());
}

void Gui::deleteControl(const std::string& i_name)
{
  d_guiControls.erase(i_name);
}
