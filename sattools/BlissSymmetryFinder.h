// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_BLISSSYMMETRYFINDER_H_
#define SATTOOLS_BLISSSYMMETRYFINDER_H_

#ifdef USE_BLISS

#include <memory>

#include "bliss/graph.hh"

#include "sattools/CNFModel.h"
#include "sattools/Group.h"
#include "sattools/Literal.h"
#include "sattools/LiteralGraphNodeAdaptor.h"

namespace sat {

class BlissSymmetryFinder {
 public:
    BlissSymmetryFinder();
    ~BlissSymmetryFinder();

    bool findAutomorphism(const CNFModel& model, Group *group);

 private:
    std::unique_ptr<LiteralGraphNodeAdaptor> _adaptor;
};

}  // namespace sat

#endif  // USE_BLISS
#endif  // SATTOOLS_BLISSSYMMETRYFINDER_H_
