#pragma once

#include <LaggySdk/Vector.h>


class Camera
{
public:
  Camera(int i_clientWidth, int i_clientHeight);

  void setLookAt(Sdk::Vector2I i_lookAt);
  const Sdk::Vector2I& getLookAt() const;

  Sdk::Vector2I getTopLeftTranslation() const;

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();

  void update(double i_dt);

private:
  const int MoveSpeed = 1000;

  const int d_clientWidth;
  const int d_clientHeight;

  Sdk::Vector2I d_lookAt{};

  int d_moveXSign = 0;
  int d_moveYSign = 0;
};
