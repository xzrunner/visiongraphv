#include "visiongraphv/RegistNodes.h"

#include <ee0/ReflectPropTypes.h>

#include <js/RTTR.h>

RTTR_REGISTRATION
{

// base

rttr::registration::class_<vgv::Node>("vgv::node")
.property("name", &vgv::Node::GetName, &vgv::Node::SetName)
(
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Name"))
)
.property("display", &vgv::Node::GetDisplay, &vgv::Node::SetDisplay)
(
	rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo(vgv::Node::STR_PROP_DISPLAY))
)
;

#define EXE_FILEPATH "visiongraphv/node_rttr_gen.h"
#define SKIP_FILE_NODE
#include "visiongraphv/node_regist_cfg.h"
#undef SKIP_FILE_NODE
#undef EXE_FILEPATH

rttr::registration::class_<vgv::node::FileInput>("vgv::file_input")
    .constructor<>()
    .property("filepath", &vgv::node::FileInput::m_filepath)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Filepath")),
        rttr::metadata(js::RTTR::FilePathTag(), true),
        rttr::metadata(ee0::PropOpenFileTag(), ee0::PropOpenFile("*.*"))
    )
;

}

namespace vgv
{

void nodes_regist_rttr()
{
}

}