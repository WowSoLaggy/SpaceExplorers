#include "stdafx.h"
#include "BuildProgressBar.h"

#include <LaggyDx/IRenderer2d.h>
#include <LaggyDx/IResourceController.h>


BuildProgressBar::BuildProgressBar(Dx::IResourceController& i_resourceController)
  : d_resourceController(i_resourceController)
{
  d_spriteFrame.setTexture(&d_resourceController.getTextureResource(FrameTextureName));
  d_spriteProgress.setTexture(&d_resourceController.getTextureResource(ProgressTextureName));
}


Sdk::Vector2I BuildProgressBar::getSize() const
{
  return d_spriteFrame.getSize();
}


void BuildProgressBar::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.renderSprite(d_spriteFrame);
  i_renderer.renderSprite(d_spriteProgress);
}


void BuildProgressBar::setObjectPosition(const Sdk::Vector2I& i_pos)
{
  d_spriteFrame.setPosition(i_pos);
  d_spriteProgress.setPosition(i_pos + Sdk::Vector2I{ 1, 1 });
}


void BuildProgressBar::setProgress(double i_progress)
{
  d_progress = i_progress;

  int frame = (int)std::floor(d_progress * 16);
  frame = std::clamp(frame, 0, 15);

  d_spriteProgress.setFrame(frame);
}
