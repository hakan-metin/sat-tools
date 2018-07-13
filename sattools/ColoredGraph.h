// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_COLOREDGRAPH_H_
#define SATTOOLS_COLOREDGRAPH_H_

#include <vector>
#include <numeric>
#include <memory>

#include "sattools/IntegralTypes.h"
#include "sattools/IntRange.h"
#include "sattools/Logging.h"

#ifdef USE_BLISS
#include "bliss/graph.hh"
#endif

namespace sat {

typedef int32 NodeIndex;

class ColoredGraph {
 public:
    ColoredGraph();
    explicit ColoredGraph(unsigned int num_nodes);
    virtual ~ColoredGraph();

    virtual void addNode(NodeIndex node) = 0;
    virtual void addEdge(NodeIndex a, NodeIndex b) = 0;
    virtual void setColor(NodeIndex node, unsigned int color) = 0;

    // int32  color(NodeIndex node) const { return _colors[node]; }
    // int32& color(NodeIndex node)       { return _colors[node]; }

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

 private:
    std::vector<std::vector<NodeIndex>> _adjacency;
    std::vector<int32> _colors;
};

#ifdef USE_BLISS
class BlissColoredGraph : public ColoredGraph {
 public:
    BlissColoredGraph();
    explicit BlissColoredGraph(unsigned int num_nodes);
    virtual ~BlissColoredGraph();

    void addNode(NodeIndex node) override;
    void addEdge(NodeIndex a, NodeIndex b) override;
    void setColor(NodeIndex node, unsigned int color) override;

 private:
    std::unique_ptr<bliss::Graph> _graph;
};
#endif

}  // namespace sat

#endif  // SATTOOLS_COLOREDGRAPH_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
