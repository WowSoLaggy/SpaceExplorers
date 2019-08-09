#pragma once

#include "Structure.h"


StructurePtr readStructureFrom(std::istream& io_stream,
                               Dx::IResourceController& i_resourceController,
                               Sdk::Vector2I i_coordsTile);
