#include "stdafx.h"
#include "SettingsProvider.h"


namespace
{
  ExternalSettings createDefaultExternalSettings()
  {
    ExternalSettings settings;
    settings.clientWidth = 1600;
    settings.clientHeight = 900;
    return settings;
  }

  InternalSettings createDefaultInternalSettings()
  {
    InternalSettings settings;
    settings.applicationName = "Space Explorers";
    settings.assetsFolder = "Data/Assets";
    settings.configFolder = "Data/Configs";
    settings.savesFolder = "Saves";
    settings.tileSize = 64;
    settings.defaultFontName = "MyFont.spritefont";
    settings.structuresPrototypesFile = "Structures.json";
    settings.objectsPrototypesFile = "Objects.json";
    return settings;
  }

} // anonymous NS


const ExternalSettings& SettingsProvider::getDefaultExternalSettings()
{
  static const auto externalSettings = createDefaultExternalSettings();
  return externalSettings;
}

const InternalSettings& SettingsProvider::getDefaultInternalSettings()
{
  static const auto internalSettings = createDefaultInternalSettings();
  return internalSettings;
}
