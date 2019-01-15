// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SYMMETRYSIMPLIFIER_H_
#define SATTOOLS_SYMMETRYSIMPLIFIER_H_

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "sattools/Assignment.h"
#include "sattools/BinaryImplicationGraph.h"
#include "sattools/BreakerManager.h"
#include "sattools/OrderManager.h"
#include "sattools/CNFModel.h"
#include "sattools/Group.h"
#include "sattools/Watcher.h"
#include "sattools/Propagator.h"
#include "sattools/Trail.h"
#include "sattools/Orbits.h"

namespace sat {

class SymmetrySimplifier {
 public:
    SymmetrySimplifier(const CNFModel &model, const Group &group,
                       const Assignment& assignment);
    ~SymmetrySimplifier();

    void simplify(ClauseInjector *injector);
    void notifyUnit(Literal unit, ClauseInjector *injector);

    void finalize();

    const Order& order() { return _order; }

 private:
    const CNFModel &_model;
    const Group &_group;
    const Assignment& _assignment;

    Orbits _orbits;
    Order _order;

    std::unique_ptr<BreakerManager> _breaker_manager;
    std::unique_ptr<OrderManager> _order_manager;

    bool addLiteralInOrderWithScore(ClauseInjector *injector);
    bool addLiteralInOrderWithUnit(Literal unit, ClauseInjector *injector);
    bool addLiteralInOrderWithOccurence();
};


}  // namespace sat


#endif  // SATTOOLS_SYMMETRYSIMPLIFIER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
