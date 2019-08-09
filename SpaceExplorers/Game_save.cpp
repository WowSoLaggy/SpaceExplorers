#include "stdafx.h"
#include "Game.h"

#include "SettingsProvider.h"

#include <LaggySdk/Contracts.h>


namespace
{
  std::string getFilename()
  {
    const std::string SaveFileName = "MyWorld.wrld";
    return ".//" + SettingsProvider::getDefaultInternalSettings().savesFolder + "//" + SaveFileName;
  }
} // anonymous NS


void Game::onSaveGame()
{
  CONTRACT_EXPECT(d_world);

  std::ofstream file(getFilename(), std::ios::binary);
  d_world->writeTo(file);
}


void Game::loadWorld()
{
  std::ifstream file(getFilename(), std::ios::binary);
  d_world = World::readFrom(file, d_camera.getViewport().size(), d_resourceController);
}
