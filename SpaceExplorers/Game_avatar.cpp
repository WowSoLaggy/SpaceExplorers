#include "stdafx.h"
#include "Game.h"

#include <LaggySdk/Contracts.h>


void Game::switchControlMode()
{
  CONTRACT_EXPECT(d_world);

  if (d_controlAvatar)
    onControlCamera();
  else
    onControlAvatar();
}

void Game::onControlAvatar()
{
  d_controlAvatar = true;

  d_avatar = d_world->getObjectPtr("Tom");
  CONTRACT_ASSERT(d_avatar);

  d_camera.setLookAt(d_avatar->getPosition());
}

void Game::onControlCamera()
{
  d_controlAvatar = false;

  if (d_avatar)
    d_avatar.reset();
}
