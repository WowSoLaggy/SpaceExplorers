#pragma once

#include "ExternalSettings.h"
#include "InternalSettings.h"


class SettingsProvider
{
public:
  static const ExternalSettings& getDefaultExternalSettings();
  static const InternalSettings& getDefaultInternalSettings();
};
