#include "stdafx.h"
#include "Camera.h"


Camera::Camera(int i_clientWidth, int i_clientHeight)
  : d_clientWidth(i_clientWidth)
  , d_clientHeight(i_clientHeight)
{
}


void Camera::lookAt(Sdk::Vector2 i_lookAt)
{
  d_lookAt = std::move(i_lookAt);
}

Sdk::Vector2 Camera::getTopLeftTranslation() const
{
  return { (float)((int)d_lookAt.x - d_clientWidth / 2), (float)((int)d_lookAt.y - d_clientHeight / 2) };
}
