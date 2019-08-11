#include "stdafx.h"
#include "Game.h"

#include <LaggySdk/Contracts.h>


void Game::switchControlMode()
{
  CONTRACT_EXPECT(d_world);

  if (isControlAvatar())
    onControlCamera();
  else
    onControlAvatar();
}

void Game::onControlAvatar()
{
  d_controlAvatar = true;

  d_avatar = d_world->getObjectPtr("Tom");
  CONTRACT_ASSERT(d_avatar);

  lookAtAvatar();
}

void Game::onControlCamera()
{
  d_controlAvatar = false;

  if (d_avatar)
    d_avatar.reset();
}

void Game::lookAtAvatar()
{
  d_camera.setLookAt(d_avatar->getPosition());
}


void Game::moveLeft()
{
  if (d_controlAvatar)
    d_avatar->moveLeft();
  else
    d_camera.moveLeft();
}

void Game::moveRight()
{
  if (d_controlAvatar)
    d_avatar->moveRight();
  else
    d_camera.moveRight();
}

void Game::moveUp()
{
  if (d_controlAvatar)
    d_avatar->moveUp();
  else
    d_camera.moveUp();
}

void Game::moveDown()
{
  if (d_controlAvatar)
    d_avatar->moveDown();
  else
    d_camera.moveDown();
}
