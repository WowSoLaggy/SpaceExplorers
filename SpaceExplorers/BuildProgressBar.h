#pragma once

#include "IGuiControl.h"

#include <LaggyDx/AnimatedSprite.h>
#include <LaggyDx/Sprite.h>
#include <LaggySdk/SdkFwd.h>


class BuildProgressBar : public IGuiControl
{
public:
  BuildProgressBar(Dx::IResourceController& i_resourceController);

  virtual void render(Dx::IRenderer2d& i_renderer) const override;

  virtual Sdk::Vector2I getSize() const override;

  void setObjectPosition(const Sdk::Vector2I& i_pos);
  void setProgress(double i_progress);

private:
  const std::string FrameTextureName = "ProgressBar_Frame.png";
  const std::string ProgressTextureName = "ProgressBar_Progress.png";

  const double Min = 0;
  const double Max = 1;

private:
  Dx::IResourceController& d_resourceController;

  Dx::Sprite d_spriteFrame;
  Dx::AnimatedSprite d_spriteProgress;

  double d_progress = Min;
};
