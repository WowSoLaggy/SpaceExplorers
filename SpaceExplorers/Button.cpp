#include "stdafx.h"
#include "Button.h"

#include "SettingsProvider.h"

#include <LaggyDx/IFontResource.h>
#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>


Button::Button(Dx::IResourceController& i_resourceController)
  : d_resourceController(i_resourceController)
  , d_font(i_resourceController.getFontResource(SettingsProvider::getDefaultInternalSettings().defaultFontName))
{
  setState(State::Normal);
}


void Button::setHandler(ButtonHandler i_handler)
{
  d_handler = i_handler;
}


void Button::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.renderSprite(d_sprite);
  if (!d_text.empty())
    i_renderer.renderText(d_text, d_font, d_textPosition);
}

Sdk::Vector2I Button::getSize() const
{
  return d_sprite.getSize();
}


void Button::setText(std::string i_text)
{
  d_text = std::move(i_text);

  const auto rect = d_font.getStringRect(d_text);
  const auto size = getSize();
  d_textOffset = (size - rect.size()) / 2;

  updateTextPosition();
}

void Button::setPosition(Sdk::Vector2I i_position)
{
  d_sprite.setPosition(std::move(i_position));
  updateRect();
  updateTextPosition();
}

void Button::updateTextPosition()
{
  d_textPosition = d_sprite.getPosition() + d_textOffset;
}

void Button::updateRect()
{
  d_rect = Sdk::RectI(d_sprite.getPosition(), d_sprite.getPosition() + d_sprite.getSize());
}


void Button::setState(State i_newState)
{
  d_state = i_newState;

  const auto& textureName = StateTextures.at(d_state);
  d_sprite.setTexture(&d_resourceController.getTextureResource(textureName));

  updateRect();
}


bool Button::onMouseClick(Dx::MouseKey i_button, const Sdk::Vector2I& i_mousePos)
{
  if (i_button == Dx::MouseKey::Left && d_rect.containsPoint(i_mousePos))
  {
    setState(State::Pressed);
    return true;
  }

  return false;
}

void Button::onMouseRelease(Dx::MouseKey i_button, const Sdk::Vector2I& i_mousePos)
{
  if (d_state == State::Pressed)
  {
    if (d_rect.containsPoint(i_mousePos))
    {
      setState(State::Light);
      press();
    }
    else
      setState(State::Normal);
  }
}

void Button::onMouseMove(const Sdk::Vector2I& i_mousePos)
{
  if (d_state == State::Normal && d_rect.containsPoint(i_mousePos))
    setState(State::Light);
  else if (d_state == State::Light && !d_rect.containsPoint(i_mousePos))
    setState(State::Normal);
}


void Button::press() const
{
  if (d_handler)
    d_handler();
}
