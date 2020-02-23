#pragma once

#include <visiongraph/typedef.h>
#include <visiongraph/CompVarType.h>

namespace visiongraph { class Device; }
namespace bp { class Node; class Pin; }

namespace vgv
{

class Evaluator;

class VgAdapter
{
public:
    static void UpdatePropBackFromFront(const bp::Node& front,
        vg::Component& back, const Evaluator& eval);

    static vg::CompPtr CreateBackFromFront(const bp::Node& front);

    static int TypeBackToFront(vg::CompVarType type);

}; // VgAdapter

}