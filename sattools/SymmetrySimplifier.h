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
#include "sattools/ClauseInjector.h"
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
    SymmetrySimplifier(const Group &group, CNFModel *model, Order *order);
    ~SymmetrySimplifier();

    void init();
    void simplify();

 private:
    const Group &_group;
    CNFModel *_model;
    Trail _trail;
    Propagator _propagator;

    Orbits _orbits;
    std::unique_ptr<BreakerManager> _breaker_manager;
    std::unique_ptr<OrderManager> _order_manager;

    std::unique_ptr<BinaryImplicationGraph> _big;

    std::unordered_map<Literal, std::vector<Clause*>> _clauses_map;

    bool addLiteralInOrderWithScore();
    bool addLiteralInOrderWithUnit(Literal unit);
    bool addUnitClause(Literal unit, bool extendsOrder);
    bool resolution(ClauseInjector *injector);
};


}  // namespace sat


#endif  // SATTOOLS_SYMMETRYSIMPLIFIER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
