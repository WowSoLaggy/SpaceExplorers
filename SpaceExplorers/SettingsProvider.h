#pragma once

#include <string>


class SettingsProvider
{
public:
  static std::string getApplicationName();
  static std::string getResourcesFolder();

  static int getClientWidth();
  static int getClientHeight();
};
