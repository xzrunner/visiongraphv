#pragma once

#include <blueprint/typedef.h>

#include <dag/Graph.h>
#include <visiongraph/typedef.h>
#include <visiongraph/CompVarType.h>
#include <node0/typedef.h>

#include <boost/noncopyable.hpp>

#include <vector>
#include <unordered_map>

namespace bp { class Connecting; }

namespace vgv
{

class Evaluator : boost::noncopyable
{
public:
    Evaluator();

    // update nodes
    void OnAddNode(const bp::Node& node, const n0::SceneNodePtr& snode,
        bool need_update = true);
    void OnRemoveNode(const bp::Node& node);
    void OnClearAllNodes();

    // update node prop
    void OnNodePropChanged(const bp::NodePtr& node);

    // update node conn
    void OnConnected(const bp::Connecting& conn);
    void OnDisconnecting(const bp::Connecting& conn);
    void OnRebuildConnection();

    auto& GetEval() const { return m_eval; }

    vg::CompPtr QueryBackNode(const bp::Node& front_node) const;

private:
    void Update();

private:
    dag::Graph<vg::CompVarType> m_eval;

    std::unordered_map<const bp::Node*, vg::CompPtr> m_front2back;

}; // Evaluator

}