// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_BREAKER_H_
#define SATTOOLS_BREAKER_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <utility>

#include "sattools/Assignment.h"
#include "sattools/BreakerInfo.h"
#include "sattools/BinaryImplicationGraph.h"
#include "sattools/CNFModel.h"
#include "sattools/ClauseInjector.h"
#include "sattools/Group.h"
#include "sattools/Literal.h"
#include "sattools/Order.h"
#include "sattools/OrderGenerator.h"
#include "sattools/Macros.h"

namespace sat {


class Breaker {

 public:
    Breaker(const CNFModel& model, const Group& group, Assignment *assignment);
    virtual ~Breaker();


    void symsimp();

    bool updateOrder(Literal literal);
    void generateSBPs();

 private:
    const CNFModel &_model;
    const Group& _group;
    Assignment *_assignment;
    ClauseInjector _injector;

    std::unique_ptr<BIG> _big;

    std::unique_ptr<OrderGenerator> _order_generator;
    std::unique_ptr<Order> _order;
    std::vector<std::unique_ptr<BreakerInfo>> _breakers;

    bool fillOrderWithScore();
    void addFullCycleInOrder(const PermCycleInfo& info, const Literal& literal);
};

}  // namespace sat
#endif  // SATTOOLS_BREAKER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
