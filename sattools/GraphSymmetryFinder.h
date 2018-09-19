// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_GRAPHSYMMETRYFINDER_H_
#define SATTOOLS_GRAPHSYMMETRYFINDER_H_

#include <vector>
#include <numeric>
#include <memory>

#include "sattools/ColoredGraph.h"
#include "sattools/IntegralTypes.h"
#include "sattools/IntRange.h"
#include "sattools/Logging.h"

#ifdef USE_BLISS
#include "bliss/graph.hh"
#endif

namespace sat {

typedef int32 NodeIndex;

class ColoredGraphSymmetryFinder : public ColoredGraph {
 public:
    ColoredGraphSymmetryFinder();
    explicit ColoredGraphSymmetryFinder(unsigned int num_nodes);
    virtual ~ColoredGraphSymmetryFinder();

    virtual void findAutomorphisms(Group *group) = 0
};

#ifdef USE_BLISS
class BlissColoredGraphSymmetryFinder : public ColoredGraphSymmetryFinder {
 public:
    BlissColoredGraphSymmetryFinder();
    explicit BlissColoredGraphSymmetryFinder(unsigned int num_nodes);
    virtual ~BlissColoredGraphSymmetryFinder();

    void findAutomorphisms(Group *group);

 private:
    std::unique_ptr<bliss::Graph> _graph;
};
#endif

}  // namespace sat

#endif  // SATTOOLS_GRAPHSYMMETRYFINDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
