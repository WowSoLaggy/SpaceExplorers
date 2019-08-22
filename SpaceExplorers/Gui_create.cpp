#include "stdafx.h"
#include "Gui.h"

#include "Button.h"
#include "Inventory.h"
#include "Game.h"
#include "Label.h"
#include "Panel.h"
#include "PrototypesCollection.h"

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
  {
    // Background
    auto panel = createPanel();
    panel->setTexture("Black.png");
    panel->setSize(d_clientSize);
  }

  {
    // New game
    auto btn = createButton();
    const auto size = btn->getSize();
    btn->setPosition({
      (d_clientSize.x - size.x) / 2,
      (d_clientSize.y - size.y) / 2 - 24 });
    btn->setText("New Game");
    btn->setHandler(std::bind(&Game::onNewGame, &d_game));
  }

  {
    // Load game
    auto btn = createButton();
    const auto size = btn->getSize();
    btn->setPosition({
      (d_clientSize.x - size.x) / 2,
      (d_clientSize.y - size.y) / 2 + 24 });
    btn->setText("Load Game");
    btn->setHandler(std::bind(&Game::onLoadGame, &d_game));
  }

  {
    // Exit
    auto btn = createButton();
    const auto size = btn->getSize();
    btn->setPosition({
      (d_clientSize.x - size.x) / 2,
      (d_clientSize.y - size.y) / 2 + 72 });
    btn->setText("Exit");
    btn->setHandler(std::bind(&Game::onExitGame, &d_game));
  }
}

void Gui::showIngameGui()
{
  // TODO: ae Nothing here so far
  /*auto inventory = createInventory("Inventory");
  const auto inventorySize = inventory->getSize();
  inventory->setPosition({ (d_clientSize.x - inventorySize.x) / 2, d_clientSize.y - inventorySize.y });*/
}


std::shared_ptr<Label> Gui::createLabel(const std::string& i_name /*= ""*/)
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

std::shared_ptr<Inventory> Gui::createInventory(Container& io_container, int i_sizeX, int i_sizeY,
                                                const std::string& i_name /*= ""*/)
{
  auto control = std::make_shared<Inventory>(d_resourceController, io_container, i_sizeX, i_sizeY);
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
  i_control->setName(std::move(i_name));
  d_controlsToAdd.push_back(i_control);
}


void Gui::deleteControl(IGuiControlPtr i_guiControl)
{
  deleteControl(i_guiControl->getName());
}

void Gui::deleteControl(const std::string& i_name)
{
  d_controlsToRemove.push_back(i_name);
}
