#include "stdafx.h"
#include "Game.h"

#include "IApp.h"
#include "Inventory.h"

#include <LaggyDx/ImageDescription.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>
#include <LaggyDx/KeyboardState.h>
#include <LaggyDx/MouseState.h>


void Game::handleKeyboard(const Dx::KeyboardState& i_keyboardState)
{
  const auto& pressedKeys = i_keyboardState.getPressedKeys();
  const auto& currentKeys = i_keyboardState.getCurrentKeys();

  if (pressedKeys.Escape)
    d_app.stop();

  if (pressedKeys.F12)
  {
    if (d_debugOutput)
      hideDebugLabel();
    else
      showDebugLabel();
  }

  if (currentKeys.A)
    d_camera.moveLeft();
  if (currentKeys.D)
    d_camera.moveRight();
  if (currentKeys.W)
    d_camera.moveUp();
  if (currentKeys.S)
    d_camera.moveDown();

  if (pressedKeys.D1)
    onSelectInventory(0);
  else if (pressedKeys.D2)
    onSelectInventory(1);
  else if (pressedKeys.D3)
    onSelectInventory(2);
  else if (pressedKeys.D4)
    onSelectInventory(3);
  else if (pressedKeys.D5)
    onSelectInventory(4);
  else if (pressedKeys.D6)
    onSelectInventory(5);
  else if (pressedKeys.D7)
    onSelectInventory(6);
  else if (pressedKeys.D8)
    onSelectInventory(7);
}

void Game::handleMouse(const Dx::MouseState& i_mouseState)
{
  const auto& mousePosRaw = i_mouseState.getPosition();
  if (i_mouseState.getMode() == Dx::MouseMode::Absolute)
    d_gui.getCursor().setPosition(mousePosRaw);
  else
    d_gui.getCursor().movePosition(mousePosRaw);

  if (i_mouseState.getLeftButtonState() == Dx::ButtonState::Pressed)
    onLClick();
  else if (i_mouseState.getRightButtonState() == Dx::ButtonState::Pressed)
    onRClick();

  if (d_buildStructure)
    updateBuildPos();
}


void Game::onLClick()
{
  if (!d_world)
    return;

  const auto& mousePos = d_gui.getCursor().getPosition();
  const auto tileCoords = screenToTile(mousePos);

  auto* tile = d_world->getTile(tileCoords);
  if (!tile)
    return;

  auto structure = tile->getTopStructure();
  if (!structure)
    return;

  structure->interact();
}

void Game::onRClick()
{
  if (!d_world)
    return;
  onSelectInventory(std::nullopt);
}


void Game::onSelectInventory(std::optional<int> i_index)
{
  auto inventory = std::dynamic_pointer_cast<Inventory>(d_gui.getControl("Inventory"));
  if (!inventory)
    return;

  if (!i_index.has_value() || inventory->getSelectedIndex() == *i_index)
    inventory->unselectItem();
  else
    inventory->selectItem(*i_index);

  onEnterBuildMode(inventory->getSelectedItem());
}

void Game::onEnterBuildMode(const StructurePrototype* i_buildStructure)
{
  d_buildStructure = i_buildStructure;
  if (!d_buildStructure)
    return;

  const auto& texture = d_resourceController.getTextureResource(d_buildStructure->textureFileName);
  d_buildSprite = Dx::Sprite{ &texture, { 0, 0 }, texture.getDescription().size(), getBuildColor() };
  updateBuildPos();
}

void Game::updateBuildPos()
{
  const auto pos = tileToScreen(screenToTile(d_gui.getCursor().getPosition()));
  d_buildSprite.setPosition(pos);
}

bool Game::canBuild() const
{
  if (!d_buildStructure)
    return false;

  return false;
}

Sdk::Vector4F Game::getBuildColor() const
{
  return canBuild() ? Sdk::Vector4F{ 0.8f, 1.5f, 0.8f, 0.7f } : Sdk::Vector4F{ 1.5f, 0.7f, 0.7f, 0.7f };
}
