// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SAT_SIMPLIFIER_H_
#define SATTOOLS_SAT_SIMPLIFIER_H_

#include <memory>
#include <vector>

#include "sattools/sat/Assignment.h"
#include "sattools/sat/BinaryImplicationGraph.h"
#include "sattools/sym/BreakerManager.h"
#include "sattools/sat/ClauseInjector.h"
#include "sattools/sym/OrderManager.h"
#include "sattools/sat/CNFModel.h"
#include "sattools/sym/Group.h"

namespace sattools {

class Simplifier {
 public:
    Simplifier(const Group &group, CNFModel *model);
    ~Simplifier();

    void simplify();

 private:
    const Group &_group;
    CNFModel *_model;
    Assignment _assignment;

    std::unique_ptr<BreakerManager> _breaker_manager;
    std::unique_ptr<OrderManager> _order_manager;

    std::unique_ptr<BinaryImplicationGraph> _big;

    bool addUnitClause(Literal unit);
    void extendsOrder(const std::vector<bool>& actives, Literal unit);
};


}  // namespace sattools


#endif  // SATTOOLS_SYM_SIMPLIFIER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
