#pragma once

#include <string>


struct InternalSettings
{
  std::string applicationName;
  std::string resourcesFolder;

  int tileSize = 0;
  std::string defaultFontName;
};
