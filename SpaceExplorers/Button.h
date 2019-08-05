#pragma once

#include "IGuiControl.h"

#include <LaggyDx/Sprite.h>
#include <LaggySdk/Vector.h>
#include <LaggySdk/Rect.h>

#include <string>
#include <unordered_map>


using ButtonHandler = std::function<void()>;


class Button : public IGuiControl
{
public:
  Button(const Dx::IResourceController& i_resourceController);

  virtual void render(Dx::IRenderer2d& i_renderer) const override;
  virtual Sdk::Vector2I getSize() const override;

  virtual void onMouseClick(Dx::MouseKey i_button, const Sdk::Vector2I& i_mousePos) override;
  virtual void onMouseRelease(Dx::MouseKey i_button, const Sdk::Vector2I& i_mousePos) override;
  virtual void onMouseMove(const Sdk::Vector2I& i_mousePos) override;

  void setPosition(Sdk::Vector2I i_position);
  void setText(std::string i_text);

  void press() const;

  void setHandler(ButtonHandler i_handler);

private:
  enum class State
  {
    Normal,
    Light,
    Pressed,
  };

  const std::unordered_map<State, std::string> StateTextures = {
    { State::Normal, "Button.png" },
    { State::Pressed, "ButtonPressed.png" },
    { State::Light, "ButtonLight.png" },
  };

private:
  const Dx::IResourceController& d_resourceController;
  Dx::Sprite d_sprite;
  State d_state = State::Normal;

  ButtonHandler d_handler = nullptr;

  std::string d_text;
  Sdk::Vector2I d_textPosition;
  Sdk::Vector2I d_textOffset;
  const Dx::IFontResource& d_font;
  void updateTextPosition();

  Sdk::RectI d_rect;
  void updateRect();

  void setState(State i_newState);
};
