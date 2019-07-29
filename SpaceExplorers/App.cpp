#include "stdafx.h"
#include "App.h"

#include "SettingsProvider.h"

#include <LaggySdk/Contracts.h>
#include <LaggySdk/HandleMessages.h>
#include <LaggySdk/Random.h>


namespace
{
  void setCursorToCenter()
  {
    int posX = GetSystemMetrics(SM_CXSCREEN) / 2;
    int posY = GetSystemMetrics(SM_CYSCREEN) / 2;
    SetCursorPos(posX, posY);
  }
}


App::App()
{
  CoInitialize(nullptr);

  Sdk::randomize();

  const auto& externalSettings = SettingsProvider::getDefaultExternalSettings();
  const auto& internalSettings = SettingsProvider::getDefaultInternalSettings();

  d_window = std::make_unique<Sdk::Window>(
    externalSettings.clientWidth, externalSettings.clientHeight, internalSettings.applicationName);

  d_renderDevice = Dx::IRenderDevice::create(
    d_window->getHWnd(), externalSettings.clientWidth, externalSettings.clientHeight);
  CONTRACT_EXPECT(d_renderDevice);

  d_resourceController = Dx::IResourceController::create(internalSettings.resourcesFolder);
  CONTRACT_EXPECT(d_resourceController);
  d_resourceController->loadResources(*d_renderDevice);

  d_renderer2d = Dx::IRenderer2d::create(*d_renderDevice);
  CONTRACT_EXPECT(d_renderer2d);

  d_inputDevice = Dx::IInputDevice::create(d_window->getHWnd());
  CONTRACT_EXPECT(d_inputDevice);

  d_game = std::make_unique<Game>(*this, *d_resourceController);

  setCursorToCenter();
  d_window->show();
}

App::~App()
{
  CoUninitialize();
}


bool App::getContinueLoop()
{
  CONTRACT_EXPECT(d_inputDevice);
  if (!Sdk::handleMessages(std::bind(&Dx::IInputDevice::processMessage, std::ref(*d_inputDevice), std::placeholders::_1)))
    return false;

  return d_continueLoop;
}

void App::stop()
{
  d_continueLoop = false;
}

void App::run()
{
  d_continueLoop = true;
  d_timer.start();

  while (getContinueLoop())
    mainloop();
}


void App::mainloop()
{
  double dt = d_timer.restart();

  d_game->handleKeyboard(d_inputDevice->checkKeyboard());
  d_game->handleMouse(d_inputDevice->checkMouse());

  d_game->update(dt);

  d_renderDevice->beginScene();
  d_renderer2d->beginScene();

  d_game->render(*d_renderer2d);

  d_renderer2d->endScene();
  d_renderDevice->endScene();
}
