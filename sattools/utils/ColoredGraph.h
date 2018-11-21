// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_UTILS_COLOREDGRAPH_H_
#define SATTOOLS_UTILS_COLOREDGRAPH_H_

#include <vector>
#include <numeric>
#include <memory>

#include "sattools/base/IntegralTypes.h"
#include "sattools/base/IntRange.h"
#include "sattools/base/Logging.h"

namespace sattools {

typedef int32 NodeIndex;

class ColoredGraph {
 public:
    ColoredGraph();
    explicit ColoredGraph(unsigned int num_nodes);
    virtual ~ColoredGraph();

    virtual void addNode(NodeIndex node) = 0;
    virtual void addEdge(NodeIndex a, NodeIndex b) = 0;
    virtual void setColor(NodeIndex node, unsigned int color) = 0;

    int64 numberOfNodes() const { return _num_nodes; }
    int64 numberOfEdges() const { return _num_edges; }

 protected:
    int64 _num_nodes;
    int64 _num_edges;
};

class AdjacencyColoredGraph : public ColoredGraph {
 public:
    AdjacencyColoredGraph();
    explicit AdjacencyColoredGraph(unsigned int num_nodes);
    virtual ~AdjacencyColoredGraph();

    void addNode(NodeIndex node) override;
    void addEdge(NodeIndex a, NodeIndex b) override;
    void setColor(NodeIndex node, unsigned int color) override;

 protected:
    std::vector<std::vector<NodeIndex>> _adjacency;
    std::vector<int32> _colors;
};

}  // namespace sattools

#endif  // SATTOOLS_UTILS_COLOREDGRAPH_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
