#include "stdafx.h"
#include "Camera.h"


Camera::Camera(int i_clientWidth, int i_clientHeight)
  : d_clientWidth(i_clientWidth)
  , d_clientHeight(i_clientHeight)
{
}


void Camera::setLookAt(Sdk::Vector2I i_lookAt)
{
  d_lookAt = std::move(i_lookAt);
}

const Sdk::Vector2I& Camera::getLookAt() const
{
  return d_lookAt;
}


Sdk::Vector2I Camera::getTopLeftPoint() const
{
  return { d_lookAt.x - d_clientWidth / 2, d_lookAt.y - d_clientHeight / 2 };
}

Sdk::Vector2I Camera::getBottomRightPoint() const
{
  return { d_lookAt.x + d_clientWidth / 2, d_lookAt.y + d_clientHeight / 2 };
}

Sdk::RectI Camera::getViewport() const
{
  return { getTopLeftPoint(), getBottomRightPoint() };
}


void Camera::moveLeft()
{
  d_moveXSign -= 1;
}

void Camera::moveRight()
{
  d_moveXSign += 1;
}

void Camera::moveUp()
{
  d_moveYSign -= 1;
}

void Camera::moveDown()
{
  d_moveYSign += 1;
}


void Camera::update(double i_dt)
{
  d_lookAt.x += (int)(MoveSpeed * i_dt * d_moveXSign);
  d_lookAt.y += (int)(MoveSpeed * i_dt * d_moveYSign);

  d_moveXSign = 0;
  d_moveYSign = 0;
}
