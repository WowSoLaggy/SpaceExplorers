#pragma once

#include "Fwd.h"

#include <LaggySdk/Vector.h>

Sdk::Vector2I screenToWorld(Sdk::Vector2I i_coords, const Camera& i_camera);
Sdk::Vector2I worldToScreen(Sdk::Vector2I i_coords, const Camera& i_camera);

Sdk::Vector2I worldToTile(Sdk::Vector2I i_coords);

Sdk::Vector2I screenToTile(Sdk::Vector2I i_coords, const Camera& i_camera);
Sdk::Vector2I tileToScreen(Sdk::Vector2I i_coords);
