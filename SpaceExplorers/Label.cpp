#include "stdafx.h"
#include "Label.h"

#include "SettingsProvider.h"

#include <LaggyDx/IResourceController.h>
#include <LaggyDx/IRenderer2d.h>


Label::Label(const Dx::IResourceController& i_resourceController)
  : d_fontResource(i_resourceController.getFontResource(SettingsProvider::getDefaultInternalSettings().defaultFontName))
{
}


void Label::render(Dx::IRenderer2d& i_renderer) const
{
  i_renderer.renderText(d_text, d_fontResource, d_position);
}
