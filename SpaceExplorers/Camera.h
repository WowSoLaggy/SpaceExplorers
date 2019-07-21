#pragma once

#include <LaggySdk/Vector.h>


class Camera
{
public:
  Camera(int i_clientWidth, int i_clientHeight);

  void lookAt(Sdk::Vector2 i_lookAt);
  Sdk::Vector2 getTopLeftTranslation() const;

private:
  const int d_clientWidth;
  const int d_clientHeight;

  Sdk::Vector2 d_lookAt{};
};
