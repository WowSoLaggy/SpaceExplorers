#include "stdafx.h"
#include "Gui.h"

#include "Button.h"
#include "Inventory.h"
#include "Game.h"
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

void Gui::showLoadingScreen()
{
  auto background = createPanel();
  background->setTexture("Black.png");
  background->setSize(d_clientSize);

  auto loading = createPanel();
  loading->setTexture("Loading.png");
  const auto loadingSize = loading->getSize();
  loading->setPosition({ (d_clientSize.x - loadingSize.x) / 2, (d_clientSize.y - loadingSize.y) / 2 });
}

void Gui::showMainMenu()
{
  auto background = createPanel();
  background->setTexture("Black.png");
  background->setSize(d_clientSize);

  auto btnNewGame = createButton();
  const auto btnNewGameSize = btnNewGame->getSize();
  btnNewGame->setPosition({ 
    (d_clientSize.x - btnNewGameSize.x) / 2, 
    (d_clientSize.y - btnNewGameSize.y) / 2 - 24});
  btnNewGame->setText("New Game");
  btnNewGame->setHandler(std::bind(&Game::onNewGame, &d_game));

  auto btnExit = createButton();
  const auto btnExitSize = btnExit->getSize();
  btnExit->setPosition({ 
    (d_clientSize.x - btnExitSize.x) / 2, 
    (d_clientSize.y - btnExitSize.y) / 2 + 24});
  btnExit->setText("Exit");
  btnExit->setHandler(std::bind(&Game::onExitGame, &d_game));
}

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
