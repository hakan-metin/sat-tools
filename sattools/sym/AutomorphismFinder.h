// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_SYM_AUTOMORPHISMFINDER_H_
#define SATTOOLS_SYM_AUTOMORPHISMFINDER_H_

#include <vector>
#include <numeric>
#include <memory>
#include <utility>

#include "sattools/utils/ColoredGraph.h"
#include "sattools/sym/Group.h"
#include "sattools/sym/LiteralGraphNodeAdaptor.h"

#include "sattools/base/IntegralTypes.h"
#include "sattools/base/IntRange.h"
#include "sattools/base/Logging.h"

// #ifdef USE_BLISS
// #include "bliss/graph.hh"
// #endif  // USE_BLISS

// #ifdef USE_SAUCY
// #include "saucy/saucy.h"
// #endif


namespace sattools {

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

}  // namespace sattools

#endif  // SATTOOLS_SYM_AUTOMORPHISMFINDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
