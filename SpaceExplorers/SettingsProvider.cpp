#include "stdafx.h"
#include "SettingsProvider.h"


std::string SettingsProvider::getApplicationName()
{
  return "Space Explorers";
}

std::string SettingsProvider::getResourcesFolder()
{
  return "Data";
}


int SettingsProvider::getClientWidth()
{
  return 1280;
}

int SettingsProvider::getClientHeight()
{
  return 720;
}
