#include "stdafx.h"
#include "Object.h"

#include "Prototypes.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>
#include <LaggyDx/ITextureResource.h>


Object::Object(
  Dx::IResourceController& i_resourceController,
  const ObjectPrototype& i_prototype)
  : d_resourceController(i_resourceController)
  , d_prototype(i_prototype)
{
  d_sprite.setTexture(&d_resourceController.getTextureResource(d_prototype.textureFileName));
  
  setPosition({ 0, 0 });
}


void Object::update(double i_dt)
{
  if (d_moveXSign != 0 || d_moveYSign != 0)
  {
    auto pos = getPosition();
    pos.x += (int)(MoveSpeed * i_dt * d_moveXSign);
    pos.y += (int)(MoveSpeed * i_dt * d_moveYSign);
    setPosition(pos);

    updateMoveAnimation();

    d_moveXSign = 0;
    d_moveYSign = 0;
  }
}

void Object::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.renderSprite(d_sprite);
}


void Object::setPosition(Sdk::Vector2I i_position)
{
  d_position = std::move(i_position);

  const auto& size = d_sprite.getSize();
  const auto topLeft = d_position - size / 2;
  d_rect = { topLeft, topLeft + size };

  d_sprite.setPosition(topLeft);
}


void Object::moveLeft()
{
  d_moveXSign -= 1;
}

void Object::moveRight()
{
  d_moveXSign += 1;
}

void Object::moveUp()
{
  d_moveYSign -= 1;
}

void Object::moveDown()
{
  d_moveYSign += 1;
}


void Object::updateMoveAnimation()
{
  if (d_moveXSign < 0)
    d_sprite.playAnimation("Left");
  else if (d_moveXSign > 0)
    d_sprite.playAnimation("Right");
  else if (d_moveYSign < 0)
    d_sprite.playAnimation("Up");
  else if (d_moveYSign > 0)
    d_sprite.playAnimation("Down");
}
