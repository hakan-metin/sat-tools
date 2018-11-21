// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_SYM_GRAPHSYMMETRYFINDER_H_
#define SATTOOLS_SYM_GRAPHSYMMETRYFINDER_H_

#include <vector>
#include <numeric>
#include <memory>

#include "sattools/utils/ColoredGraph.h"
#include "sattools/base/IntegralTypes.h"
#include "sattools/base/IntRange.h"
#include "sattools/base/Logging.h"

namespace sattools {

typedef int32 NodeIndex;

class ColoredGraphSymmetryFinder : public ColoredGraph {
 public:
    ColoredGraphSymmetryFinder();
    explicit ColoredGraphSymmetryFinder(unsigned int num_nodes);
    virtual ~ColoredGraphSymmetryFinder();

    virtual void findAutomorphisms(Group *group) = 0
};


}  // namespace sattools

#endif  // SATTOOLS_SYM_GRAPHSYMMETRYFINDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
