#pragma once

#include <cstdint>
#include <string>


enum class Gas : std::int32_t
{
  Oxygen = 0,
  CarbonDioxide,
};

const std::unordered_map<Gas, std::string> GasNames{
  { Gas::Oxygen, "O2" },
  { Gas::CarbonDioxide, "CO2" },
};
