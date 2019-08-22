#pragma once

#include <cstdint>


enum class Action : int32_t
{
  None = 0,
  Default,
  Open,
  Close,
};
