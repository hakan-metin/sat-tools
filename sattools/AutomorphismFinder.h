// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_AUTOMORPHISMFINDER_H_
#define SATTOOLS_AUTOMORPHISMFINDER_H_

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

// #ifdef USE_BLISS
// #include "bliss/graph.hh"
// #endif  // USE_BLISS

// #ifdef USE_SAUCY
// #include "saucy/saucy.h"
// #endif


namespace sat {

using Adaptor = std::unique_ptr<LiteralGraphNodeAdaptor>;

struct AutomorphismInfo {
    AutomorphismInfo(unsigned int n, LiteralGraphNodeAdaptor* a, Group *g)
        : num_vars(n), adaptor(a), group(g) {}
    unsigned int num_vars;
    LiteralGraphNodeAdaptor *adaptor;
    Group *group;
};


class AutomorphismFinder {
    virtual void findAutomorphisms(unsigned int num_vars,
                                   const Adaptor& adaptor,
                                   Group *group) = 0;
};

}  // namespace sat

#endif  // SATTOOLS_AUTOMORPHISMFINDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
