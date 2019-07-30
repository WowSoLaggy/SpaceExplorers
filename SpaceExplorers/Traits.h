#pragma once

#include <cstdint>


enum class Behavior : int32_t
{
  None = 0,
  Door,
};


enum class Layer : int32_t
{
  None = 0,
  Panneling,
  Floor,
  Wall,
};
