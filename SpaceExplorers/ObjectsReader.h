#pragma once

#include "Fwd.h"
#include "Object.h"
#include "Structure.h"

#include <LaggyDx/LaggyDxFwd.h>
#include <LaggySdk/Vector.h>


StructurePtr readStructureFrom(std::istream& io_stream,
                               Dx::IResourceController& i_resourceController,
                               Sdk::Vector2I i_coordsTile);

ObjectPtr readObjectFrom(std::istream& io_stream,
                         Dx::IResourceController& i_resourceController,
                         const World& i_world);
