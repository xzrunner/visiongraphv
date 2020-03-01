#include "visiongraphv/Evaluator.h"
#include "visiongraphv/VgAdapter.h"
#include "visiongraphv/Node.h"

#include <blueprint/Node.h>
#include <blueprint/Pin.h>
#include <blueprint/Connecting.h>
#include <blueprint/CompNode.h>

#include <visiongraph/Component.h>

#include <queue>

namespace vgv
{

Evaluator::Evaluator()
{
}

void Evaluator::OnAddNode(const bp::Node& front, const n0::SceneNodePtr& snode, bool need_update)
{
    auto back = VgAdapter::CreateBackFromFront(front);
    if (!back) {
        return;
    }

    m_eval.AddNode(back);

    m_front2back.insert({ &front, back });

    if (front.get_type().is_derived_from<Node>()) {
        const_cast<Node&>(static_cast<const Node&>(front)).SetName(back->GetName());
    }

    VgAdapter::UpdatePropBackFromFront(front, *back, *this);
    if (need_update) {
        Update();
    }
}

void Evaluator::OnRemoveNode(const bp::Node& node)
{
    auto itr = m_front2back.find(&node);
    if (itr == m_front2back.end()) {
        return;
    }

    m_eval.RemoveNode(itr->second);
    m_front2back.erase(itr);

    Update();
}

void Evaluator::OnClearAllNodes()
{
    m_eval.ClearAllNodes();
    m_front2back.clear();

    Update();
}

void Evaluator::OnNodePropChanged(const bp::NodePtr& node)
{
    auto itr = m_front2back.find(node.get());
    // not visiongraph node
    if (itr == m_front2back.end()) {
        return;
    }

    VgAdapter::UpdatePropBackFromFront(*node, *itr->second, *this);

    if (node->get_type().is_derived_from<Node>())
    {
        auto& terrv_n = std::static_pointer_cast<Node>(node);
        if (terrv_n->GetName() != itr->second->GetName()) {
            //m_eval.Rename(itr->second->GetName(), terrv_n->GetName());
            if (itr->second->GetName() != terrv_n->GetName()) {
                terrv_n->SetName(itr->second->GetName());
            }
        }
    }

    m_eval.PropChanged(itr->second);
    //m_eval.MakeDirty();

    Update();
}

void Evaluator::OnConnected(const bp::Connecting& conn)
{
    auto f_pin = conn.GetFrom();
    auto t_pin = conn.GetTo();

    auto f_itr = m_front2back.find(&f_pin->GetParent());
    auto t_itr = m_front2back.find(&t_pin->GetParent());
    if (f_itr == m_front2back.end() || t_itr == m_front2back.end()) {
        return;
    }

    //if (t_itr->first->GetAllInput().size() > t_itr->second->GetImports().size()) {
    //    t_itr->second->AddInputPorts(t_itr->first->GetAllInput().size() - t_itr->first->GetAllOutput().size());
    //}

    m_eval.Connect(
        { f_itr->second, f_pin->GetPosIdx() },
        { t_itr->second, t_pin->GetPosIdx() }
    );

    Update();
}

void Evaluator::OnDisconnecting(const bp::Connecting& conn)
{
    auto f_pin = conn.GetFrom();
    auto t_pin = conn.GetTo();

    auto f_itr = m_front2back.find(&f_pin->GetParent());
    auto t_itr = m_front2back.find(&t_pin->GetParent());
    if (f_itr == m_front2back.end() || t_itr == m_front2back.end()) {
        return;
    }

    m_eval.Disconnect(
        { f_itr->second, f_pin->GetPosIdx() },
        { t_itr->second, t_pin->GetPosIdx() }
    );

    Update();
}

void Evaluator::OnRebuildConnection()
{
    std::vector<std::pair<vg::Component::PortAddr, vg::Component::PortAddr>> conns;
    for (auto& itr : m_front2back)
    {
        auto& front = itr.first;
        auto& back  = itr.second;
        for (auto& in : front->GetAllInput())
        {
            for (auto& conn : in->GetConnecting())
            {
                auto f_pin = conn->GetFrom();
                auto t_pin = conn->GetTo();

                auto f_itr = m_front2back.find(&f_pin->GetParent());
                auto t_itr = m_front2back.find(&t_pin->GetParent());
                if (f_itr == m_front2back.end() || t_itr == m_front2back.end()) {
                    continue;
                }

                //if (t_itr->first->GetAllInput().size() > t_itr->second->GetImports().size()) {
                //    t_itr->second->AddInputPorts(t_itr->first->GetAllInput().size() - t_itr->first->GetAllOutput().size());
                //}

                conns.push_back({
                    { f_itr->second, f_pin->GetPosIdx() },
                    { t_itr->second, t_pin->GetPosIdx() }
                });
            }
        }
    }

    m_eval.RebuildConnections(conns);

    Update();
}

vg::CompPtr Evaluator::QueryBackNode(const bp::Node& front_node) const
{
    auto itr = m_front2back.find(&front_node);
    return itr == m_front2back.end() ? nullptr : itr->second;
}

void Evaluator::Update()
{
    m_eval.Update();
}

}