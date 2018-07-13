// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_COLOREDGRAPH_H_
#define SATTOOLS_COLOREDGRAPH_H_

#include <vector>
#include <numeric>

#include "sattools/IntegralTypes.h"
#include "sattools/IntRange.h"

namespace sat {

typedef int NodeIndex;

class ColoredGraph {
 public:
    ColoredGraph();
    explicit ColoredGraph(unsigned int num_node);
    virtual ~ColoredGraph();

    void addNode(NodeIndex node);
    void addEdge(NodeIndex a, NodeIndex b);

    IntRange nodes() const { return IntRange(0, numberOfNodes()); }
    const std::vector<NodeIndex>& neighbour(NodeIndex node) const;
    int32  degree(NodeIndex node) const { return neighbour(node).size(); }

    int32  color(NodeIndex node) const { return _colors[node]; }
    int32& color(NodeIndex node)       { return _colors[node]; }

    int64 numberOfNodes() const { return _num_nodes; }
    int64 numberOfEdges() const { return _num_edges; }

 private:
    int64 _num_nodes;
    int64 _num_edges;
    std::vector<std::vector<NodeIndex>> _adjacency;
    std::vector<int32> _colors;
};


}  // namespace sat

#endif  // SATTOOLS_COLOREDGRAPH_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
