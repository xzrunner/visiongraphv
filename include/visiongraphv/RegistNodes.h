#pragma once

#include "visiongraphv/Node.h"

namespace vgv
{

void nodes_regist_rttr();

namespace node
{

#define EXE_FILEPATH "visiongraphv/node_def_gen.h"
#include "visiongraphv/node_regist_cfg.h"
#undef EXE_FILEPATH

}

}