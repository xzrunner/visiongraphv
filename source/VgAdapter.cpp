#include "visiongraphv/VgAdapter.h"
#include "visiongraphv/PinType.h"
#include "visiongraphv/Node.h"

#include <blueprint/Node.h>
#include <blueprint/Pin.h>

#include <visiongraph/Component.h>

namespace vgv
{

void VgAdapter::UpdatePropBackFromFront(const bp::Node& front, vg::Component& back,
                                        const Evaluator& eval)
{
    auto f_type = front.get_type();
    auto b_type = back.get_type();
    if (f_type.is_derived_from<Node>() &&
        b_type.is_derived_from<vg::Component>())
    {
        for (auto& dst_prop : b_type.get_properties())
        {
            auto src_prop = f_type.get_property(dst_prop.get_name());
            assert(src_prop.is_valid());
            dst_prop.set_value(back, src_prop.get_value(front));
        }
    }
}

vg::CompPtr VgAdapter::CreateBackFromFront(const bp::Node& node)
{
    auto type = node.get_type();
    auto src_type = type.get_name().to_string();
    std::string dst_type;
    std::string lib_str = "vg";
    auto find_lib = src_type.find("vgv::");
    if (find_lib != std::string::npos) {
        dst_type = lib_str + "::" + src_type.substr(find_lib + strlen("vgv::"));
    }

    vg::CompPtr dst = nullptr;

    if (!dst_type.empty())
    {
	    rttr::type t = rttr::type::get_by_name(dst_type);
        // fixme: specify node type
	    if (!t.is_valid())
        {
            assert(0);
	    }
        else
        {
            rttr::variant var = t.create();
            assert(var.is_valid());

            dst = var.get_value<std::shared_ptr<vg::Component>>();
            assert(dst);
        }
    }

    if (node.get_type().is_derived_from<Node>()) {
        dst->SetName(static_cast<const Node&>(node).GetName());
    }

    return dst;
}


int VgAdapter::TypeBackToFront(vg::CompVarType type)
{
    int ret = -1;

    switch (type)
    {
    case vg::CompVarType::Any:
        ret = bp::PIN_ANY_VAR;
        break;
    case vg::CompVarType::Image:
        ret = PIN_IMAGE;
        break;
    default:
        assert(0);
    }

    return ret;
}

}