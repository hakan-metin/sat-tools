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
}

ColoredGraph::~ColoredGraph() {
}

/* -------------------------------------------------------------------------- */

AdjacencyColoredGraph::AdjacencyColoredGraph() : ColoredGraph() {
}

AdjacencyColoredGraph::AdjacencyColoredGraph(unsigned int num_nodes)
    : ColoredGraph(num_nodes) {
    _adjacency.resize(num_nodes);
    _colors.resize(num_nodes);
}

AdjacencyColoredGraph::~AdjacencyColoredGraph() {
}

void AdjacencyColoredGraph::addNode(NodeIndex node) {
    if (_num_nodes > node)
        return;
    _num_nodes = node + 1;
    _adjacency.resize(_num_nodes);
    _colors.resize(_num_nodes);
}

void AdjacencyColoredGraph::addEdge(NodeIndex a, NodeIndex b) {
    addNode(a > b ? a : b);
    _adjacency[a].push_back(b);
    _adjacency[b].push_back(a);
    _num_edges++;
}

void AdjacencyColoredGraph::setColor(NodeIndex node, unsigned int color) {
    CHECK_LT(node, _num_nodes);
    _colors[node] = color;
}

/* -------------------------------------------------------------------------- */

BlissColoredGraph::BlissColoredGraph() : ColoredGraph(),
                                         _graph(new bliss::Graph(_num_nodes)) {
}

BlissColoredGraph::BlissColoredGraph(unsigned int num_nodes)
    : ColoredGraph(num_nodes),
      _graph(new bliss::Graph(num_nodes)) {
    LOG(INFO) << "I AM BLISS";
}

BlissColoredGraph::~BlissColoredGraph() {
}

void BlissColoredGraph::addNode(NodeIndex node) {
    unsigned int n;
    while (_num_nodes <= node) {
        n = _graph->add_vertex();
        CHECK_EQ(n, _num_nodes);
        _num_nodes = node + 1;
    }
}

void BlissColoredGraph::addEdge(NodeIndex a, NodeIndex b) {
    _graph->add_edge(a, b);
    _num_edges++;
}

void BlissColoredGraph::setColor(NodeIndex node, unsigned int color) {
    CHECK_LT(node, _num_nodes);
    _graph->change_color(node, color);
}

}  // namespace sat
