#pragma once

#include <string>


struct InternalSettings
{
  std::string applicationName;
  std::string assetsFolder;
  std::string configFolder;
  std::string savesFolder;

  int tileSize = 0;
  std::string defaultFontName;

  std::string structuresPrototypesFile;
  std::string objectsPrototypesFile;
};
