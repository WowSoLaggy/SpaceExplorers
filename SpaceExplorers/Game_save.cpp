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
  file << *d_world;
}
