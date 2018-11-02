// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SIMPLIFIER_H_
#define SATTOOLS_SIMPLIFIER_H_

#include <memory>

#include "sattools/Assignment.h"
#include "sattools/BinaryImplicationGraph.h"
#include "sattools/Breaker.h"
#include "sattools/CNFModel.h"
#include "sattools/Group.h"

namespace sat {

class Simplifier {
 public:
    Simplifier(const Group &group, CNFModel *model);
    ~Simplifier();

 private:
    const Group &_group;
    CNFModel *_model;
    Assignment _assignment;

    std::unique_ptr<Breaker> _breaker;
    std::unique_ptr<BIG> _big;

};


}  // namespace sat


#endif  // SATTOOLS_SIMPLIFIER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
