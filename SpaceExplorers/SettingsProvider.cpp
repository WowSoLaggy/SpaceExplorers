#include "stdafx.h"
#include "SettingsProvider.h"


namespace
{
  ExternalSettings createDefaultExternalSettings()
  {
    ExternalSettings settings;
    settings.clientWidth = 1280;
    settings.clientHeight = 720;
    return settings;
  }

  InternalSettings createDefaultInternalSettings()
  {
    InternalSettings settings;
    settings.applicationName = "Space Explorers";
    settings.resourcesFolder = "Data";
    settings.tileSize = 64;
    settings.defaultFontName = "MyFont.spritefont";
    return settings;
  }

} // anonymous NS


const ExternalSettings& SettingsProvider::getDefaultExternalSettings()
{
  static const ExternalSettings externalSettings = createDefaultExternalSettings();
  return externalSettings;
}

const InternalSettings& SettingsProvider::getDefaultInternalSettings()
{
  static const InternalSettings internalSettings = createDefaultInternalSettings();
  return internalSettings;
}
