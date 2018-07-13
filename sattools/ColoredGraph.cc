// Copyright 2017 Hakan Metin

#include "sattools/ColoredGraph.h"

namespace sat {

ColoredGraph::ColoredGraph() :
    _num_nodes(0),
    _num_edges(0) {
}

ColoredGraph::ColoredGraph(unsigned int num_nodes) :
    _num_nodes(num_nodes),
    _num_edges(0) {
    _adjacency.resize(_num_nodes);
    _colors.resize(_num_nodes);
}

ColoredGraph::~ColoredGraph() {
}

void ColoredGraph::addNode(NodeIndex node) {
    if (_num_nodes > node)
        return;
    _num_nodes = node + 1;
    _adjacency.resize(_num_nodes);
    _colors.resize(_num_nodes);
}

void ColoredGraph::addEdge(NodeIndex a, NodeIndex b) {
    addNode(a > b ? a : b);
    _adjacency[a].push_back(b);
    _adjacency[b].push_back(a);
    _num_edges++;
}

const std::vector<NodeIndex>& ColoredGraph::neighbour(NodeIndex node) const {
    return _adjacency[node];
}


}  // namespace sat
