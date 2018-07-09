// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SYMMETRYFINDER_H_
#define SATTOOLS_SYMMETRYFINDER_H_

#include <string>

#include "sattools/CNFModel.h"
#include "sattools/Group.h"
#include "sattools/LiteralGraphNodeAdaptor.h"

namespace sat {
template<typename Graph>
class SymmetryFinder : private Graph {
    using Graph::addNode;
 public:
    SymmetryFinder() {}
    ~SymmetryFinder() {}

    void buildGraph(const CNFModel& model);

 private:
    Graph _graph;
};

template<typename Graph>
inline void SymmetryFinder<Graph>::buildGraph(const CNFModel& model) {
    _graph.addNode(2);
}

}  // namespace sat

#endif  // SATTOOLS_SYMMETRYFINDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
