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
  Sdk::randomize();

  d_window = std::make_unique<Sdk::Window>(
    SettingsProvider::getClientWidth(), SettingsProvider::getClientHeight(), "Space Explorers");
  CONTRACT_EXPECT(d_window);

  d_renderDevice = Dx::IRenderDevice::create(
    d_window->getHWnd(), SettingsProvider::getClientWidth(), SettingsProvider::getClientHeight());
  CONTRACT_EXPECT(d_renderDevice);

  d_resourceController = Dx::IResourceController::create(SettingsProvider::getResourcesFolder());
  CONTRACT_EXPECT(d_resourceController);

  d_resourceController->loadResources(*d_renderDevice);
  CONTRACT_EXPECT(d_resourceController);

  d_renderer2d = Dx::IRenderer2d::create(*d_renderDevice, *d_resourceController);
  CONTRACT_EXPECT(d_renderer2d);

  d_inputDevice = Dx::IInputDevice::create(d_window->getHWnd());
  CONTRACT_EXPECT(d_inputDevice);

  setCursorToCenter();
  d_window->show();
}


bool App::getContinueLoop()
{
  CONTRACT_EXPECT(d_inputDevice);
  if (!Sdk::handleMessages(std::bind(&Dx::IInputDevice::processMessage, std::ref(*d_inputDevice), std::placeholders::_1)))
    return false;

  return d_continueLoop;
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

  const auto& keyboardState = d_inputDevice->checkKeyboard();
  //handleKeyboard(keyboardState);
  const auto& mouseState = d_inputDevice->checkMouse();
  //handleMouse(mouseState);

  //checkLogic(dt);

  d_renderDevice->beginScene();
  d_renderer2d->beginScene();

  //d_viewModel->render(*d_renderer2d, mouseState.getMousePosition());
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  d_renderer2d->endScene();
  d_renderDevice->endScene();
}
