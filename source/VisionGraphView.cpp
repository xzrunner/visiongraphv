#include "visiongraphv/VisionGraphView.h"
#include "visiongraphv/PinCallback.h"
#include "visiongraphv/Node.h"

#include <blueprint/NodeBuilder.h>
#include <blueprint/node/Commentary.h>

#include <visiongraph/VisionGraph.h>

namespace vgv
{

CU_SINGLETON_DEFINITION(VisionGraphView);

extern void regist_rttr();

VisionGraphView::VisionGraphView()
{
	vg::VisionGraph::Instance();

	regist_rttr();

	InitNodes();

    InitPinCallback();
}

void VisionGraphView::InitNodes()
{
    const int bp_count = 1;

	auto list = rttr::type::get<Node>().get_derived_classes();
	m_nodes.reserve(bp_count + list.size());

    m_nodes.push_back(std::make_shared<bp::node::Commentary>());

	for (auto& t : list)
	{
		auto obj = t.create();
		assert(obj.is_valid());
		auto node = obj.get_value<bp::NodePtr>();
		assert(node);
		m_nodes.push_back(node);
	}
}

}