#pragma once

#include <cstdint>


enum class Layer : int32_t
{
  None = 0,
  Panneling,
  Floor,
  Wall,
  Attachment,
};

static Layer getPrevLayer(Layer i_layer)
{
  return static_cast<Layer>(static_cast<int>(i_layer) - 1);
}
