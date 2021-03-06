#include "stdafx.h"
#include "Game.h"

#include "Gui.h"
#include "Label.h"
#include "PowerLoadModel.h"
#include "PowerSourceModel.h"
#include "Utils.h"


void Game::showDebugLabel()
{
  d_debugLabel = d_gui.createLabel("DebugLabel");
  d_debugOutput = true;
}

void Game::hideDebugLabel()
{
  d_debugOutput = false;
  d_gui.deleteControl(d_debugLabel);
  d_debugLabel.reset();
}


void Game::updateDebugLabel(int i_renderedSprites) const
{
  if (!d_debugLabel)
    return;

  std::string str = "Sprites: " + std::to_string(i_renderedSprites) + "\n";

  str += "Cursor coords: " + std::to_string(d_gui.getCursor().getPosition().x) + "x, " +
    std::to_string(d_gui.getCursor().getPosition().y) + "y\n";

  if (d_world)
  {
    const auto tileCoords = screenToTile(d_gui.getCursor().getPosition(), d_camera);
    str += "Cursor tile coords: " + std::to_string(tileCoords.x) + "x, " +
      std::to_string(tileCoords.y) + "y\n";

    if (const auto* tile = d_world->getTile(tileCoords))
    {
      const auto& atmo = tile->getAtmosphere();
      const auto ratios = atmo.getGasesRatios();

      str += "========================\n";
      str += "Pressure: " + std::to_string(atmo.getPressure()) + " Pa\n";
      for (const auto&[type, amount] : atmo.getGases())
      {
        const int ratio = (int)(ratios.at(type) * 100);
        str += "--- " + GasNames.at(type) + ": " + std::to_string(amount) +
          " (" + std::to_string(ratio) + " %)\n";
      }

      //

      const auto relativeCoords =
        screenToWorld(d_gui.getCursor().getPosition(), d_camera) -
        getTileRect(tileCoords).topLeft();
      if (const auto structure = tile->getStructure(relativeCoords))
      {
        if (const auto powerSourceModel = structure->getBehaviorModel<PowerSourceModel>())
        {
          str += "========================\n";

          str += "Power: " + std::to_string(powerSourceModel->getCurPower()) +
            "W / " + std::to_string(powerSourceModel->getMaxPower()) + "W\n";
        }

        if (const auto powerLoadModel = structure->getBehaviorModel<PowerLoadModel>())
        {
          str += "========================\n";

          str += "Req. Power: " + std::to_string(powerLoadModel->getRequiredPower()) + "W\n";
        }
      }
    }
  }

  d_debugLabel->setText(str);
}
