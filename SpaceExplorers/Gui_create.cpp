#include "stdafx.h"
#include "Gui.h"

#include "Inventory.h"
#include "Label.h"
#include "Prototypes.h"


void Gui::createIngameGui()
{
  auto inventory = createInventory();
  const auto inventorySize = inventory->getSize();
  inventory->setPosition({ (d_clientSize.x - inventorySize.x) / 2, d_clientSize.y - inventorySize.y });

  inventory->setItem(0, Prototypes::Lattice());
  inventory->setItem(1, Prototypes::Floor());
  inventory->setItem(2, Prototypes::Wall());
  inventory->setItem(3, Prototypes::Door());
}


std::shared_ptr<Label> Gui::createDebugLabel()
{
  auto control = std::make_shared<Label>(d_resourceController);
  d_guiControls["DebugLabel"] = control;
  return control;
}

std::shared_ptr<Inventory> Gui::createInventory()
{
  auto control = std::make_shared<Inventory>(d_resourceController);
  d_guiControls["Inventory"] = control;
  return control;
}
