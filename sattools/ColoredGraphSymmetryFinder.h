// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_COLOREDGRAPHSYMMETRYFINDER_H_
#define SATTOOLS_COLOREDGRAPHSYMMETRYFINDER_H_

#include <vector>
#include <numeric>
#include <memory>
#include <utility>

#include "sattools/ColoredGraph.h"
#include "sattools/Group.h"
#include "sattools/LiteralGraphNodeAdaptor.h"

#include "sattools/IntegralTypes.h"
#include "sattools/IntRange.h"
#include "sattools/Logging.h"

#ifdef USE_BLISS
#include "bliss/graph.hh"
#endif

namespace sat {

struct SymmetryFinderInfo {
    SymmetryFinderInfo(unsigned int n, LiteralGraphNodeAdaptor* a, Group *g)
        : num_vars(n), adaptor(a), group(g) {}
    unsigned int num_vars;
    LiteralGraphNodeAdaptor *adaptor;
    Group *group;
};

class ColoredGraphSymmetryFinder : public ColoredGraph {
 public:
    ColoredGraphSymmetryFinder() : ColoredGraph() {}
    explicit ColoredGraphSymmetryFinder(unsigned int num_nodes) :
        ColoredGraph(num_nodes) {}

    virtual ~ColoredGraphSymmetryFinder() {}

    virtual void
    findAutomorphisms(unsigned int num_vars,
                      const std::unique_ptr<LiteralGraphNodeAdaptor>& adaptor,
                      Group *group) = 0;
};

#ifdef USE_BLISS

class BlissColoredGraphSymmetryFinder : public ColoredGraphSymmetryFinder {
 public:
    BlissColoredGraphSymmetryFinder() : ColoredGraphSymmetryFinder() {}
    explicit BlissColoredGraphSymmetryFinder(unsigned int num_nodes) :
        ColoredGraphSymmetryFinder(num_nodes) {
        _graph = std::make_unique<bliss::Graph>(num_nodes); }
    virtual ~BlissColoredGraphSymmetryFinder() {}

    void addNode(NodeIndex node) override;
    void addEdge(NodeIndex a, NodeIndex b) override;
    void setColor(NodeIndex node, unsigned int color) override;

    void
    findAutomorphisms(unsigned int num_vars,
                      const std::unique_ptr<LiteralGraphNodeAdaptor>& adaptor,
                      Group *group) override;

 private:
    std::unique_ptr<bliss::Graph> _graph;
};
#endif

}  // namespace sat

#endif  // SATTOOLS_COLOREDGRAPHSYMMETRYFINDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
